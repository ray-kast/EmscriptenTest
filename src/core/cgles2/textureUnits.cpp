#include "textureUnits.hpp"

#include <diag.hpp>

namespace cgl {
BindTexture TextureUnits::addTex(std::size_t texId,
                                 GLenum      unit,
                                 GLenum      target) {
  auto &&tex = m_texs[texId];

  if (!m_bindings.emplace(unit, Binding{.tex = &tex, .target = target}).second)
    die("binding for texture unit " + std::to_string(unit) +
        " already created");

  return BindTexture(unit, target, tex);
}

BindTexture TextureUnits::bindTex(GLenum unit) const {
  auto &&bind = m_bindings.at(unit);
  return BindTexture(unit, bind.target, *bind.tex);
}

SelectTextureUnits::SelectTextureUnits(const TextureUnits &units) :
    m_units(&units) {
  for (auto &[unit, binding] : units.m_bindings)
    m_binds.emplace_back(unit, binding.target, *binding.tex);
}
} // namespace cgl
