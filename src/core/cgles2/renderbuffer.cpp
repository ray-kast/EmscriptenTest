#include "renderbuffer.hpp"

#include <unordered_set>

#include <diag.hpp>

#include "texture.hpp"

namespace cgl {
static std::unordered_set<GLenum> s_bound;

BindRenderbuffer::BindRenderbuffer(GLenum target, const Renderbuffer &rbuf) :
    m_target(target),
    m_rbuf(&rbuf) {
  if (!s_bound.emplace(m_target).second)
    die("renderbuffer type " + glEnumName(target) + " already bound");

  glBindRenderbuffer(target, rbuf.m_rbuf);
}

BindRenderbuffer::~BindRenderbuffer() {
  if (m_target.empty()) return;
  glBindRenderbuffer(m_target, 0);
  s_bound.erase(m_target);
}

void BindRenderbuffer::storage(GLenum format, GLsizei width, GLsizei height) {
  glRenderbufferStorage(m_target, format, width, height);
}
} // namespace cgl
