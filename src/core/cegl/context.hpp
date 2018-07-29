#pragma once

#include <vector>

#include <EGL/egl.h>

#include <linear.hpp>

#include "surface.hpp"

namespace cegl {
class Context {
  const Display *                          m_disp;
  core::Linear<EGLContext, EGL_NO_CONTEXT> m_ctx;

public:
  Context() {}

  Context(const Surface &, const std::vector<EGLint> &);

  Context &operator=(Context &&) = default;

  ~Context();

  friend class MakeCurrent;
};
} // namespace cegl
