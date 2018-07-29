#include "display.hpp"

#include <stdexcept>

#include <diag.hpp>

namespace cegl {
Display::Display(const cx::Display &disp) {
  m_disp = eglGetDisplay(
#if defined(_JS)
      reinterpret_cast<EGLNativeDisplayType>(disp.m_disp.get())
#else
      disp.m_disp
#endif
  );

  if (m_disp == EGL_NO_DISPLAY)
    throw std::runtime_error("eglGetDisplay failed");

  if (!eglInitialize(m_disp, &m_major, &m_minor))
    throw std::runtime_error("eglInitialize failed");
}

std::vector<EGLConfig> Display::getConfigs() const {
  EGLint count;

  if (!eglGetConfigs(m_disp, nullptr, 0, &count))
    throw std::runtime_error("eglGetConfigs query failed");

  std::vector<EGLConfig> ret(count);

  if (!eglGetConfigs(m_disp, &ret[0], ret.size(), &count))
    throw std::runtime_error("eglGetConfigs retrieval failed");

  return ret;
}

Display::~Display() {
  if (m_disp.empty()) return;
  if (!eglTerminate(m_disp)) err("eglTerminate failed");
}
} // namespace cegl
