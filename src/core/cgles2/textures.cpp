#include "textures.hpp"

namespace cgl {
Textures::Textures(GLsizei size) : m_size(size), m_texs(new GLuint[size]) {
  glGenTextures(size, m_texs);
}

Textures::~Textures() {
  if (m_texs.empty()) return;
  glDeleteTextures(m_size, m_texs);
  delete[] m_texs;
  m_texs = nullptr;
}

const Texture &Textures::operator[](GLsizei idx) {
  auto it = m_owned.find(idx);

  if (it == m_owned.end())
    it = m_owned.emplace(idx, Texture(m_texs[idx])).first;

  return it->second;
}
} // namespace cgl
