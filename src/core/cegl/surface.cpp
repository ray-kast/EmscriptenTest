#include "surface.hpp"

#include <diag.hpp>
#include <getPtr.hpp>

#include "attribList.hpp"

namespace cegl {
Surface::Surface(const Display &            disp,
                 EGLConfig                  config,
                 const cx::Window &         win,
                 const std::vector<EGLint> &attribs) :
    m_disp(&disp),
    m_conf(config) {
  auto attribList = makeAttribList(attribs);
  m_surf          = eglCreateWindowSurface(disp.m_disp,
                                  config,
                                  win.m_win,
                                  _getVecPtr(attribList));

  if (m_surf == EGL_NO_SURFACE) die("eglCreateWindowSurface failed");
}

Surface::~Surface() {
  if (m_surf.empty()) return;
  if (!eglDestroySurface(m_disp->m_disp, m_surf))
    err("eglDestroySurface failed");
}

void Surface::swap() { eglSwapBuffers(m_disp->m_disp, m_surf); }
} // namespace cegl
