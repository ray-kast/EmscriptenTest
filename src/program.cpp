#include "program.hpp"

#include <GLES2/gl2.h>
#include <diag.hpp>

Program::Program(int, char **) {
  m_xDisp   = cx::Display(nullptr);
  auto root = cx::Window::root(m_xDisp);
  m_win     = cx::Window(root, 0, 0, 640, 480);

  m_disp = cegl::Display(m_xDisp);

  // TODO: this is real stupid
  auto &&configs = m_disp.getConfigs();
  info("Got " + std::to_string(configs.size()) + " config(s).");

  m_surf = cegl::Surface(m_disp, configs[0], m_win, {});
  m_ctx  = cegl::Context(m_surf, {EGL_CONTEXT_CLIENT_VERSION, 2});

  m_makeCurrent = cegl::MakeCurrent(m_surf, m_surf, m_ctx);
}

Program::~Program() {}

bool Program::mainLoop() {
  glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_surf.swap();

  return true;
}
