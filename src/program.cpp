#include "program.hpp"

#include <GLES2/gl2.h>

#include <diag.hpp>

#include <cegl/configInfo.hpp>

Program::Program(int, char **) {
  m_xDisp   = cx::Display(nullptr);
  auto root = cx::Window::root(m_xDisp);
  m_win     = cx::Window(root, 0, 0, 640, 480);

  m_disp = cegl::Display(m_xDisp);

  info("using EGL version " + std::to_string(m_disp.majorVer()) + "." +
       std::to_string(m_disp.minorVer()));

  auto &&configs = m_disp.chooseConfig({
      // clang-format off
      EGL_RED_SIZE,       8,
      EGL_GREEN_SIZE,     8,
      EGL_BLUE_SIZE,      8,
      EGL_ALPHA_SIZE,     EGL_DONT_CARE,
      EGL_DEPTH_SIZE,     24,
      EGL_STENCIL_SIZE,   EGL_DONT_CARE,
      EGL_SAMPLES,        4,
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

  glGenBuffers(1, &m_buf);

  glBindBuffer(GL_ARRAY_BUFFER, m_buf);

  GLfloat data[]{
      // clang-format off
       0.0f,  0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
      // clang-format on
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

Program::~Program() { glDeleteBuffers(1, &m_buf); }

bool Program::mainLoop() {
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  {
    cgl::UseProgram pgm(m_blit);

    glBindBuffer(GL_ARRAY_BUFFER, m_buf);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  m_surf.swap();

  return true;
}
