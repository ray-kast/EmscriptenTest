#include "renderbuffers.hpp"

namespace cgl {
Renderbuffers::Renderbuffers(GLsizei size) :
    m_size(size),
    m_rbufs(new GLuint[size]) {
  glGenRenderbuffers(size, m_rbufs);
}

Renderbuffers::~Renderbuffers() {
  if (m_rbufs.empty()) return;
  glDeleteRenderbuffers(m_size, m_rbufs);
  delete[] m_rbufs;
  m_rbufs = nullptr;
}

const Renderbuffer &Renderbuffers::operator[](GLsizei idx) {
  auto it = m_owned.find(idx);

  if (it == m_owned.end())
    it = m_owned.emplace(idx, Renderbuffer(m_rbufs[idx])).first;

  return it->second;
}
} // namespace cgl
