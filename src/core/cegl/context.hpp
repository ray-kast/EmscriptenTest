#pragma once

#include <EGL/egl.h>

#include <nocopy.hpp>

namespace cx {
class Window;
}

namespace cegl {
class Context {
public:
  CORE_NOCOPY_MOVE(Context);

  Context(const cx::Window &);
};
} // namespace cegl
