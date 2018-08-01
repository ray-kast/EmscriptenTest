#include "display.hpp"

#include <diag.hpp>
#include <getPtr.hpp>

#include "attribList.hpp"

namespace cegl {
Display::Display(const cx::Display &disp) {
  m_disp = eglGetDisplay(
#if defined(_JS)
      reinterpret_cast<EGLNativeDisplayType>(disp.m_disp.get())
#else
      disp.m_disp
#endif
  );

  if (m_disp == EGL_NO_DISPLAY) die("eglGetDisplay failed");

  if (!eglInitialize(m_disp, &m_major, &m_minor)) die("eglInitialize failed");
}

std::vector<EGLConfig> Display::getConfigs() const {
  EGLint count;

  if (!eglGetConfigs(m_disp, nullptr, 0, &count))
    die("eglGetConfigs query failed");

  std::vector<EGLConfig> ret(count);

  if (!eglGetConfigs(m_disp, _getVecPtr(ret), ret.size(), &count))
    die("eglGetConfigs retrieval failed");

  return ret;
}

std::vector<EGLConfig> Display::chooseConfig(
    const std::vector<EGLint> &attribs) const {
  EGLint count;

  if (!eglGetConfigs(m_disp, nullptr, 0, &count))
    die("eglChooseConfig query failed");

  std::vector<EGLConfig> ret(count);

  auto attribList = makeAttribList(attribs);
  if (!eglChooseConfig(
          m_disp, _getVecPtr(attribList), _getVecPtr(ret), count, &count))
    die("eglChooseConfig retrieval failed");

  ret.resize(count);

  return ret;
}

Display::~Display() {
  if (m_disp.empty()) return;
  if (!eglTerminate(m_disp)) err("eglTerminate failed");
}
} // namespace cegl
