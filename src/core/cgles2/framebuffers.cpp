#include "framebuffers.hpp"

namespace cgl {
Framebuffers::Framebuffers(GLsizei size) :
    m_size(size),
    m_fbufs(new GLuint[size]) {
  glGenFramebuffers(size, m_fbufs);
}

Framebuffers::~Framebuffers() {
  if (m_fbufs.empty()) return;
  glDeleteFramebuffers(m_size, m_fbufs);
  delete[] m_fbufs;
  m_fbufs = nullptr;
}

const Framebuffer &Framebuffers::operator[](GLsizei idx) {
  auto it = m_owned.find(idx);

  if (it == m_owned.end())
    it = m_owned.emplace(idx, Framebuffer(m_fbufs[idx])).first;

  return it->second;
}
} // namespace cgl
