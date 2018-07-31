#pragma once

#include <list>
#include <map>

#include "textures.hpp"

namespace cgl {
class TextureUnits {
  struct Binding {
    const Texture *tex;
    GLenum         target;
  };

  Textures                  m_texs;
  std::map<GLenum, Binding> m_bindings;

public:
  TextureUnits() {}

  TextureUnits(GLsizei size) : m_texs(size) {}

  const Texture &addTex(std::size_t texId, GLenum unit, GLenum target);

  TextureUnits &operator=(TextureUnits &&) = default;

  const Texture &operator[](std::size_t i) { return m_texs[i]; }

  friend class SelectTextureUnits;
};

class SelectTextureUnits {
  const TextureUnits *   m_units;
  std::list<BindTexture> m_binds;

public:
  SelectTextureUnits() {}

  explicit SelectTextureUnits(const TextureUnits &);

  SelectTextureUnits(const SelectTextureUnits &) = delete;
};
} // namespace cgl
