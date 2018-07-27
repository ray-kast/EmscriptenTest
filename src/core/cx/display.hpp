#pragma once

#include <X11/Xlib.h>

#include <nocopy.hpp>

namespace cegl {
class Context;
}

namespace cx {
class Window;

class Display {
  ::Display *m_disp;

public:
  CORE_NOCOPY_MOVE(Display);

  explicit Display(const char *);

  friend class Window;
  friend class cegl::Context;
};
} // namespace cx
