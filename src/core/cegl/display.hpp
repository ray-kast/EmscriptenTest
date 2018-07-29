#pragma once

#include <vector>

#include <EGL/egl.h>

#include <linear.hpp>

#include <cx/display.hpp>

namespace cegl {
class Display {
  core::Linear<EGLDisplay, EGL_NO_DISPLAY> m_disp;
  EGLint                                   m_major, m_minor;

public:
  // WHY THE HELL ARE major AND minor DEFINED AS MACROS?!
  constexpr const auto &majorVer() const { return m_major; }
  constexpr const auto &minorVer() const { return m_minor; }

  Display() : m_major(0), m_minor(0) {}

  explicit Display(const cx::Display &);

  ~Display();

  std::vector<EGLConfig> getConfigs() const;

  std::vector<EGLConfig> chooseConfig(const std::vector<EGLint> &) const;

  Display &operator=(Display &&) = default;

  friend class ConfigInfo;
  friend class Context;
  friend class MakeCurrent;
  friend class Surface;
};
} // namespace cegl
