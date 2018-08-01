#include "context.hpp"

#include <iostream>

#include <diag.hpp>
#include <getPtr.hpp>

#include <cx/display.hpp>
#include <cx/window.hpp>

#include "attribList.hpp"

namespace cegl {
Context::Context(const Surface &surf, const std::vector<EGLint> &attribs) :
    m_disp(surf.m_disp) {
  auto attribList = makeAttribList(attribs);
  m_ctx           = eglCreateContext(surf.m_disp->m_disp,
                           surf.m_conf,
                           EGL_NO_CONTEXT,
                           _getVecPtr(attribList));

  if (m_ctx == EGL_NO_CONTEXT) die("eglCreateContext failed");
}

Context::~Context() {
  if (m_ctx.empty()) return;
  if (!eglDestroyContext(m_disp->m_disp, m_ctx))
    err("eglDestroyContext failed");
}
} // namespace cegl
