#include "framebuffer.hpp"

#include <unordered_set>

#include <diag.hpp>

#include "texture.hpp"

namespace cgl {
static std::unordered_set<GLenum> s_bound;

BindFramebuffer::BindFramebuffer(GLenum target, const Framebuffer &fbuf) :
    m_target(target) {
  if (!s_bound.emplace(m_target).second)
    die("framebuffer type " + std::to_string(target) + " already bound");

  glBindFramebuffer(target, fbuf.m_fbuf);
}

BindFramebuffer::~BindFramebuffer() {
  if (m_target.empty()) return;
  glBindFramebuffer(m_target, 0);
  s_bound.erase(m_target);
}

void BindFramebuffer::texture2D(GLenum             attachment,
                                const BindTexture &tex,
                                GLint              lvl) {
  glFramebufferTexture2D(
      m_target, attachment, tex.m_target, tex.m_tex->m_tex, lvl);
}

void BindFramebuffer::assertStatus() {
  auto status = glCheckFramebufferStatus(m_target);

  if (status != GL_FRAMEBUFFER_COMPLETE)
    die("framebuffer " + std::to_string(m_target) + " had unexpected status " +
        std::to_string(status));
}
} // namespace cgl
