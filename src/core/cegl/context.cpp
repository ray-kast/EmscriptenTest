#include "context.hpp"

#include <iostream>
#include <stdexcept>

#include <diag.hpp>

#include <cx/display.hpp>
#include <cx/window.hpp>

namespace cegl {
Context::Context(const Surface &surf, const std::vector<EGLint> &attribs) :
    m_disp(surf.m_disp) {
  std::vector<EGLint> attribList(attribs.size() + 1);
  attribList.insert(attribList.begin(), attribs.begin(), attribs.end());
  attribList[attribs.size()] = EGL_NONE;

  m_ctx = eglCreateContext(surf.m_disp->m_disp,
                           surf.m_conf,
                           EGL_NO_CONTEXT,
                           &attribList[0]);

  if (m_ctx == EGL_NO_CONTEXT)
    throw std::runtime_error("eglCreateContext failed");
}

Context::~Context() {
  if (m_ctx.empty()) return;
  if (!eglDestroyContext(m_disp->m_disp, m_ctx))
    err("eglDestroyContext failed");
}
} // namespace cegl
