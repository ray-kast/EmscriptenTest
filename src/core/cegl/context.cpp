#include "context.hpp"

#include <stdexcept>

#include <cx/display.hpp>
#include <cx/window.hpp>

namespace cegl {
Context::Context(const cx::Window &win) {
  EGLDisplay display = eglGetDisplay(
#if defined(_JS)
      // Pretty sure this doesn't even matter
      reinterpret_cast<EGLNativeDisplayType>(win.m_disp->m_disp)
#else
      win.m_disp->m_disp
#endif
  );

  if (display == EGL_NO_DISPLAY)
    throw std::runtime_error("eglGetDisplay failed");

  EGLint major, minor;

  if (!eglInitialize(display, &major, &minor))
    throw std::runtime_error("eglInitialize failed");
}
} // namespace cegl
