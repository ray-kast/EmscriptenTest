#include "display.hpp"

#include <diag.hpp>

namespace cx {
Display::Display(const char *name) {
  m_disp = XOpenDisplay(name);

  if (!m_disp) die("XOpenDisplay failed");
}

Display::~Display() {
  if (m_disp.empty()) return;

#if !defined(_JS)
  XCloseDisplay(m_disp);
#endif
}

Atom Display::internAtom(const std::string &str) const {
  return XInternAtom(m_disp, str.c_str(), false);
}

std::string Display::atomName(Atom atom) const {
  return XGetAtomName(m_disp, atom);
}

bool Display::pending() const { return XPending(m_disp); }

XEvent Display::nextEvent() {
  XEvent ret;
  XNextEvent(m_disp, &ret);
  return ret;
}
} // namespace cx
