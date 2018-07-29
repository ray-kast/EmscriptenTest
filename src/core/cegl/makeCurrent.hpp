#pragma once

#include <linear.hpp>

#include "display.hpp"

namespace cegl {
class Context;
class Surface;

class MakeCurrent {
  core::Linear<const Display *, nullptr> m_disp;

public:
  MakeCurrent() {}

  MakeCurrent(const Surface &draw, const Surface &read, const Context &);

  ~MakeCurrent();

  MakeCurrent &operator=(MakeCurrent &&) = default;
};
} // namespace cegl
