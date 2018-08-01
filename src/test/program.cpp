#include "program.hpp"

#include <array>
#include <random>

#include <GLES2/gl2.h>

#undef Success // I hate that Xlib does this.

#include <Eigen/Eigen>

#include <diag.hpp>
#include <matrixMath.hpp>
#include <path.hpp>
#include <transformStack.hpp>

#include <cegl/configInfo.hpp>

#include <models.hpp>

using namespace pth::lit;

Program::Program(int, char **) : m_particles(1024) {
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

  m_particle = cgl::Material();

  {
    cgl::SetupMaterial setup(m_particle);

    setup.add(GL_VERTEX_SHADER, "assets/shd/particle.vert");
    setup.add(GL_FRAGMENT_SHADER, "assets/shd/particle.frag");

    setup.input(0, "in_POSITION");
  }

  // Model setup

  mdl::setupBlitQuad(m_bkgdQuad);
  mdl::blitQuad(m_bkgdQuad,
                1.0f,
                Eigen::Vector3f(0.0f, 0.0f, 0.0f),
                cgl::FreqStatic);

  mdl::setupFanPath(m_circle);

  mdl::setupStrokePath(m_fieldLines);

  {
    pth::Path path;

    path.open(pth::Vec(0.0f, 1.0f))
        .arc(pth::Vec(0.0f, -1.0f), 1.0f)
        .arc(pth::Vec(0.0f, 1.0f), 1.0f)
        .close();

    mdl::fanPath(
        m_circle, path, 64, Eigen::Vector3f(1.0f, 1.0f, 1.0f), cgl::FreqStatic);
  }

  // Texture setup

  m_white = cgl::TextureUnits(1);
  m_white.addTex(0, 0, GL_TEXTURE_2D)
      .color(Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

  resize(START_W, START_H);
}

Program::~Program() {}

inline pth::Vec ortho(const pth::Vec &v) {
  return v.reverse().cwiseProduct(pth::Vec(-1.0f, 1.0f));
}

inline pth::Vec particleField(const pth::Vec &pos) {
  pth::Vec a = pos - pth::Vec(-0.75f, 0.0f), b = pos - pth::Vec(0.75f, 0.0f);

  pth::Vec aField = 0.25f * ortho(a) + 0.5f * a;
  aField.normalize();
  aField *= 1.0f / std::sqrt(1.0f + a.squaredNorm());

  pth::Vec bField = -0.45f * ortho(b) + -0.65f * b;
  bField.normalize();
  bField *= 1.0f / std::sqrt(1.0f + b.squaredNorm());

  return aField + bField;
}

void Program::render(double time) {
  double dt = time - m_lastTime;

  glViewport(0, 0, m_width, m_height);

  glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
  glClearDepthf(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  {
    cgl::UseProgram         pgm(m_blit);
    cgl::SelectTextureUnits tex(m_white);

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

  std::mt19937_64 mt(std::random_device{}());

  int spawns = 10;

  TransformStack ts;

  Eigen::Vector2f aspect(m_width, m_height);
  aspect.normalize();

  const float SCALE = 4.0f;

  ts << Eigen::Ortho3f(SCALE * aspect, 0.0f, 100.0f);

  Eigen::Projective3f iproj = ts->inverse();

  while (time > m_spawnTime) {
    --spawns;

    if (m_particles[m_nextParticle].remain > 1e-3f)
      warn("killing particle " + std::to_string(m_nextParticle));

    const float JITTER = 0.03f;

    std::uniform_real_distribution<float> posJitter(-JITTER, JITTER);

    float life = std::uniform_real_distribution<float>(1.0f, 3.0f)(mt);

    m_particles[m_nextParticle] = Particle{
        // .pos = (iproj *
        //         Eigen::Vector3f(
        //             std::uniform_real_distribution<float>(-1.0f, 1.0f)(mt),
        //             std::uniform_real_distribution<float>(-1.0f, 1.0f)(mt),
        //             0.0f)
        //             .homogeneous())
        //            .hnormalized()
        //            .template head<2>(),
        .pos = Eigen::Vector2f(-0.77f, -0.03f) +
               Eigen::Vector2f(posJitter(mt), posJitter(mt)),
        .clr = Eigen::Vector3f(
            std::uniform_real_distribution<float>(0.0f, 1.0f)(mt),
            std::uniform_real_distribution<float>(0.0f, 1.0f)(mt),
            std::uniform_real_distribution<float>(0.0f, 1.0f)(mt)),
        .size   = std::uniform_real_distribution<float>(0.05f, 0.13f)(mt),
        .life   = life,
        .remain = life,
    };

    m_nextParticle = (m_nextParticle + 1) % m_particles.size();

    m_spawnTime = (spawns ? m_spawnTime : time) +
                  std::uniform_real_distribution(0.0f, 0.03f)(mt);
  }

  // if (int count = 10 - spawns; count) info(std::to_string(count));

  // Field lines

  // {
  //   cgl::UseProgram         pgm(m_blit);
  //   cgl::SelectTextureUnits tex(m_white);

  //   pgm.uniform("u_S2D_TEXTURE").set(0);

  //   pth::Path field;

  //   // TODO: something about this is slow
  //   for (int r = 0; r < 11; ++r) {
  //     for (int c = 0; c < 11; ++c) {
  //       pth::Vec pos(pth::lerp(-1.0_sc, 1.0_sc, c / 10_sc),
  //                    pth::lerp(-1.0_sc, 1.0_sc, r / 10_sc));

  //       field.open(pos);

  //       for (int i = 0; i < 10; ++i) {
  //         pos += particleField(pos) * 0.02f;
  //         field.line(pos);
  //       }
  //     }
  //   }

  //   mdl::strokePath(m_fieldLines,
  //                   field,
  //                   4,
  //                   0.005f,
  //                   Eigen::Vector3f(1.0f, 0.0f, 0.0f),
  //                   cgl::FreqDynamic);

  //   ts.save();

  //   ts << Eigen::Translation3f(Eigen::Vector3f(0.0f, 0.0f, -0.1f));

  //   pgm.uniform("u_MAT_TRANSFORM").set(*ts, false);

  //   cgl::SelectModel(m_fieldLines).draw();

  //   ts.restore();
  // }

  // Particles

  {
    cgl::UseProgram         pgm(m_particle);
    cgl::SelectModel        mdl(m_circle);

    pgm.uniform("u_MAT_PROJ").set(*ts, false);

    ts.save()->setIdentity();

    pgm.uniform("u_MAT_VIEW").set(*ts, false);

    ts->setIdentity();

    for (int i = 0; i < m_particles.size(); ++i) {
      auto &&particle = m_particles[i];

      if (particle.remain < 1e-3f) continue;

      particle.remain -= dt;

      ts.save();

      Eigen::Vector3f pos;
      Eigen::Vector4f clr;

      pos.template head<2>() = particle.pos;
      pos.z() = 0.0f;

      clr.template head<3>() = particle.clr;
      clr.w() = 1.0f;

      ts << Eigen::Translation3f(pos);

      ts << Eigen::UniformScaling<float>(
          pth::lerp(0.0f, particle.size, particle.remain / particle.life));

      pgm.uniform("u_MAT_WORLD").set(*ts, false);
      pgm.uniform("u_VEC_COLOR").set(clr);

      mdl.draw();

      ts.restore();

      particle.pos += particleField(particle.pos) * dt;
    }

    ts.restore();
  }

  m_surf.swap();

  m_lastTime = time;
}

void Program::resize(int width, int height) {
  m_width  = width;
  m_height = height;
}
