#pragma once

#include <vector>

#include <EGL/egl.h>

#include <linear.hpp>

#include <cx/window.hpp>

#include "display.hpp"

namespace cegl {
class Surface {
  const Display *                          m_disp;
  core::Linear<EGLSurface, EGL_NO_SURFACE> m_surf;
  EGLConfig                                m_conf; // TODO: is this problematic?

public:
  Surface() : m_disp(nullptr), m_conf{} {}

  Surface(const Display &,
          EGLConfig,
          const cx::Window &,
          const std::vector<EGLint> &);

  ~Surface();

  void swap();

  Surface &operator=(Surface &&) = default;

  friend class Context;
  friend class MakeCurrent;
};
} // namespace cegl
