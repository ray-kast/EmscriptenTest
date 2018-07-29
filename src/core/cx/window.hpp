#pragma once

#include <X11/Xlib.h>

#include <linear.hpp>

namespace cegl {
class Surface;
}

namespace cx {
class Display;

class Window {
  const Display *           m_disp;
  core::Linear<::Window, 0> m_win;

  Window(const Display &disp, ::Window win) : m_disp(&disp), m_win(win) {}

public:
  static Window root(const Display &);

  Window() : m_disp(nullptr) {}

  Window(const Window &, int x, int y, unsigned int width, unsigned int height);

  // TODO: does this need a destructor?

  Window &operator=(Window &&) = default;

  friend class cegl::Surface;
};
} // namespace cx
