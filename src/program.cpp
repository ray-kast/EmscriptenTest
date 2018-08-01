#include "program.hpp"

#include <array>

#include <GLES2/gl2.h>

#undef Success // I hate that Xlib does this.

#include <Eigen/Eigen>

#include <diag.hpp>
#include <matrixMath.hpp>
#include <path.hpp>
#include <transformStack.hpp>

#include <cegl/configInfo.hpp>

#include <models.hpp>

Program::Program(int, char **) {
  const unsigned int START_W = 640, START_H = 480;

  m_xDisp   = cx::Display(nullptr);
  auto root = cx::Window::root(m_xDisp);
  m_win     = cx::Window(root, 0, 0, START_W, START_H);

  m_win.title("a window");

  m_disp = cegl::Display(m_xDisp);

  info("using EGL version " + std::to_string(m_disp.majorVer()) + "." +
       std::to_string(m_disp.minorVer()));

  auto &&configs = m_disp.chooseConfig({
      // clang-format off
      EGL_RED_SIZE,     8,
      EGL_GREEN_SIZE,   8,
      EGL_BLUE_SIZE,    8,
      EGL_ALPHA_SIZE,   EGL_DONT_CARE,
      EGL_DEPTH_SIZE,   24,
      EGL_STENCIL_SIZE, EGL_DONT_CARE,
      EGL_SAMPLES,      4,
      // clang-format on
  });

  auto &&config = configs[0];

  info("using config " +
       std::to_string(cegl::ConfigInfo(m_disp, config), true));

  m_surf = cegl::Surface(m_disp, config, m_win, {});
  m_ctx  = cegl::Context(m_surf, {EGL_CONTEXT_CLIENT_VERSION, 2});

  m_makeCurrent = cegl::MakeCurrent(m_surf, m_surf, m_ctx);

  // Material setup

  m_blit = cgl::Material();

  {
    cgl::SetupMaterial setup(m_blit);

    setup.add(GL_VERTEX_SHADER, "assets/shd/blit.vert");
    setup.add(GL_FRAGMENT_SHADER, "assets/shd/blit.frag");

    setup.input(0, "in_POSITION");
    setup.input(1, "in_COLOR");
    setup.input(2, "in_UV0");
  }

  // Model setup

  mdl::setupBlitQuad(m_bkgdQuad);
  mdl::blitQuad(m_bkgdQuad,
                1.0f,
                Eigen::Vector3f(1.0f, 1.0f, 1.0f),
                cgl::FreqStatic);

  mdl::setupStrokePath(m_worm);

  // Texture setup

  m_white = cgl::TextureUnits(1);

  cgl::BindTexture(0, GL_TEXTURE_2D, m_white.addTex(0, 0, GL_TEXTURE_2D))
      .color(Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

  m_wood = cgl::TextureUnits(1);

  {
    cgl::BindTexture tex(0, GL_TEXTURE_2D, m_wood.addTex(0, 0, GL_TEXTURE_2D));

    tex.loadImage(0, "assets/tex/wood.jpg");
  }

  resize(START_W, START_H);
}

Program::~Program() {}

void Program::render(double time) {
  auto timef = static_cast<float>(time);

  glViewport(0, 0, m_width, m_height);

  glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
  glClearDepthf(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  {
    cgl::UseProgram         pgm(m_blit);
    cgl::SelectTextureUnits tex(m_wood);

    pgm.uniform("u_MAT_TRANSFORM").set(Eigen::Projective3f::Identity(), false);
    pgm.uniform("u_S2D_TEXTURE").set(0);

    cgl::SelectModel(m_bkgdQuad).draw();
  }

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glCullFace(GL_BACK);
  glDepthFunc(GL_LESS);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  {
    pth::Path path;

    path.open(pth::Vec(-0.75f, 0.0f))
        .bezier(pth::Vec(-0.35f, pth::lerp(0.0f, 0.5f, std::sin(timef))),
                pth::Vec(0.35f, pth::lerp(0.0f, 0.5f, std::cos(timef))),
                pth::Vec(0.75f, 0.0f));

    mdl::strokePath(m_worm,
                    path,
                    64,
                    0.1f,
                    Eigen::Vector3f(1.0f, 0.75f, 0.5f),
                    cgl::FreqDynamic);
  }

  {
    cgl::UseProgram         pgm(m_blit);
    cgl::SelectTextureUnits tex(m_wood);

    TransformStack ts;

    Eigen::Vector2f aspect(m_width, m_height);
    aspect.normalize();

    const float SCALE = 4.0f;

    ts << Eigen::Ortho3f(SCALE * aspect, 0.0f, 100.0f);

    ts << Eigen::AngleAxisf(timef / 4.0f, Eigen::Vector3f::UnitZ());

    pgm.uniform("u_MAT_TRANSFORM").set(*ts, false);

    cgl::SelectModel(m_worm).draw();
  }

  m_surf.swap();
}

void Program::resize(int width, int height) {
  m_width  = width;
  m_height = height;
}
