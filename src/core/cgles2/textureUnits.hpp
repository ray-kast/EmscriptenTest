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

  BindTexture addTex(std::size_t texId, GLenum unit, GLenum target);

  BindTexture bindTex(GLenum unit) const;

  TextureUnits &operator=(TextureUnits &&) = default;

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
