#pragma once

#include <X11/Xlib.h>

#include <linear.hpp>

namespace cegl {
class Display;
}

namespace cx {
class Display {
  core::Linear<::Display *, nullptr> m_disp;

public:
  Display() {}

  explicit Display(const char *);

  ~Display();

  Display &operator=(Display &&) = default;

  friend class Window;
  friend class cegl::Display;
};
} // namespace cx
