#pragma once

#include <X11/Xlib.h>

#include <nocopy.hpp>

namespace cegl {
class Context;
}

namespace cx {
class Display;

class Window {
  const Display *m_disp;
  ::Window       m_win;

  Window(const Display &disp, ::Window win) : m_disp(&disp), m_win(win) {}

public:
  static Window root(const Display &);

  CORE_NOCOPY_ASG_MOVE(Window);

  explicit Window(const Window &);

  friend class cegl::Context;
};
} // namespace cx
