#include "makeCurrent.hpp"

#include <stdexcept>

#include <EGL/egl.h>

#include <diag.hpp>

#include "context.hpp"
#include "surface.hpp"

namespace cegl {
// TODO: Does this need to be a map over the display?
static bool s_hasCurrent = false;

MakeCurrent::MakeCurrent(const Surface &draw,
                         const Surface &read,
                         const Context &ctx) :
    m_disp(ctx.m_disp) {
  if (draw.m_disp->m_disp != read.m_disp->m_disp ||
      read.m_disp->m_disp != ctx.m_disp->m_disp)
    throw std::runtime_error("draw, read, and context displays don't match");

  if (s_hasCurrent) throw std::runtime_error("current context already set");
  s_hasCurrent = true;

  if (!eglMakeCurrent(ctx.m_disp->m_disp, draw.m_surf, read.m_surf, ctx.m_ctx))
    throw std::runtime_error("eglMakeCurrent failed");
}

MakeCurrent::~MakeCurrent() {
  if (m_disp.empty()) return;

  if (!eglMakeCurrent(
          m_disp.get()->m_disp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT))
    err("eglMakeCurrent failed");

  s_hasCurrent = false;
}
} // namespace cegl
