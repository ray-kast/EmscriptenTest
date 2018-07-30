#include "program.hpp"

#include <GLES2/gl2.h>

#include <diag.hpp>

#include <cegl/configInfo.hpp>

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

  m_blit = cgl::Material();

  {
    cgl::SetupMaterial setup(m_blit);

    setup.add(GL_VERTEX_SHADER, "assets/shd/blit.vert");
    setup.add(GL_FRAGMENT_SHADER, "assets/shd/blit.frag");

    setup.input(0, "in_POSITION");
  }

  m_triangle = cgl::Model(GL_TRIANGLES, 2);

  {
    GLfloat data[]{
        // clang-format off
        0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        // clang-format on
    };

    m_triangle.loadVbuf(data,
                        sizeof(data),
                        cgl::FreqStatic,
                        cgl::AccessDraw,
                        0,
                        3,
                        GL_FLOAT,
                        0,
                        nullptr);
  }

  {
    GLushort data[]{0, 1, 2};

    m_triangle.loadIbuf(data,
                        sizeof(data),
                        cgl::FreqStatic,
                        cgl::AccessDraw,
                        sizeof(*data),
                        GL_UNSIGNED_SHORT,
                        nullptr);
  }

  resize(START_W, START_H);
}

Program::~Program() {}

void Program::render() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glCullFace(GL_BACK);
  glDepthFunc(GL_LESS);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
  glClearDepthf(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, m_width, m_height);

  {
    cgl::UseProgram  pgm(m_blit);
    cgl::SelectModel mdl(m_triangle);

    mdl.draw();
  }

  m_surf.swap();
}

void Program::resize(int width, int height) {
  m_width = width;
  m_height = height;
}
