#include "textureUnits.hpp"

#include <diag.hpp>

namespace cgl {
const Texture &TextureUnits::addTex(std::size_t texId,
                                    GLenum      unit,
                                    GLenum      target) {
  auto &&tex = m_texs[texId];

  if (!m_bindings.emplace(unit, Binding{.tex = &tex, .target = target}).second)
    die("texture unit " + std::to_string(unit) + " already bound");

  return tex;
}

SelectTextureUnits::SelectTextureUnits(const TextureUnits &units) :
    m_units(&units) {
  for (auto &[unit, binding] : units.m_bindings)
    m_binds.emplace_back(unit, binding.target, *binding.tex);
}
} // namespace cgl
