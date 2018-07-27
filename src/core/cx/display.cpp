#include "display.hpp"

#include <stdexcept>

namespace cx {
Display::Display(const char *name) {
  m_disp = XOpenDisplay(name);

  if (!m_disp) throw std::runtime_error("XOpenDisplay failed");
}
} // namespace cx
