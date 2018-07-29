#include "display.hpp"

#include <stdexcept>

#include <diag.hpp>

namespace cx {
Display::Display(const char *name) {
  m_disp = XOpenDisplay(name);

  if (!m_disp) throw std::runtime_error("XOpenDisplay failed");
}

Display::~Display() {
  if (m_disp.empty()) return;

#if defined(_JS)
// Nothing to do here (probably)
// This is mainly because XCloseDisplay apparently doesn't exist here
#else
  if (XCloseDisplay(m_disp)) err("XCloseDisplay failed");
#endif
}
} // namespace cx
