#pragma once

#include <unordered_map>

#include <GLES2/gl2.h>

#include <linear.hpp>

#include "texture.hpp"

namespace cgl {
class Textures {
  GLsizei                              m_size;
  core::Linear<GLuint *, nullptr>      m_texs;
  std::unordered_map<GLsizei, Texture> m_owned;

public:
  constexpr const auto &size() const { return m_size; }

  Textures() : m_size(0) {}

  Textures(GLsizei);

  ~Textures();

  Textures &operator=(Textures &&) = default;

  const Texture &operator[](GLsizei);
};
} // namespace cgl
