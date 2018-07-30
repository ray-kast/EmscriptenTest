#include "buffers.hpp"

#include <GLES2/gl2ext.h>

#include <diag.hpp> // TODO: remove

namespace cgl {
Buffers::Buffers(GLsizei size) : m_size(size), m_bufs(new GLuint[size]) {
  glGenBuffers(size, m_bufs);
}

Buffers::~Buffers() {
  if (m_bufs.empty()) return;
  glDeleteBuffers(m_size, m_bufs);
  delete[] m_bufs;
  m_bufs = nullptr;
}

const Buffer &Buffers::operator[](GLsizei idx) {
  auto it = m_owned.find(idx);

  if (it == m_owned.end()) it = m_owned.emplace(idx, Buffer(m_bufs[idx])).first;

  return it->second;
}
} // namespace cgl
