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
      EGL_SAMPLE_BUFFERS, 0,
      // clang-format on
  });

  auto &&config = configs[0];

  info("using config " +
       std::to_string(cegl::ConfigInfo(m_disp, config), true));

  m_surf = cegl::Surface(m_disp, config, m_win, {});
  m_ctx  = cegl::Context(m_surf, {EGL_CONTEXT_CLIENT_VERSION, 2});

  m_makeCurrent = cegl::MakeCurrent(m_surf, m_surf, m_ctx);

  m_mat = cgl::Material();

  {
    cgl::SetupMaterial setup(m_mat);

    setup.add(GL_VERTEX_SHADER,
              "#version 100\n"

              "attribute vec3 in_POSITION;\n"

              "void main() {\n"
              "  gl_Position = vec4(in_POSITION, 1.0);\n"
              "}\n");

    setup.add(GL_FRAGMENT_SHADER,
              "#version 100\n"

              "precision mediump float;\n"

              "void main() {\n"
              "  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
              "}\n");

    setup.input(0, "in_POSITION");
  }
}

Program::~Program() {}

bool Program::mainLoop() {
  glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  { cgl::UseProgram pgm(m_mat); }

  m_surf.swap();

  return true;
}
