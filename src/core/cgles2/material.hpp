#pragma once

#include <cassert>
#include <functional>
#include <string>
#include <unordered_map>

#include <linear.hpp>

#include "program.hpp"
#include "shader.hpp"

namespace cgl {
class Material {
  cgl::Program m_pgm;
  bool         m_valid;

public:
  constexpr auto &pgm() {
    assert(m_valid);
    return m_pgm;
  }

  constexpr const auto &pgm() const {
    assert(m_valid);
    return m_pgm;
  }

  Material() : m_valid(false) {}

  constexpr operator Program &() { return pgm(); }
  constexpr operator const Program &() const { return pgm(); }

  friend class SetupMaterial;
};

class SetupMaterial {
  Material *                                                 m_mat;
  std::unordered_map<GLenum, std::pair<Shader, std::string>> m_shaders;

public:
  explicit SetupMaterial(Material &);

  SetupMaterial(const SetupMaterial &) = delete;

  ~SetupMaterial();

  void add(GLenum type, const std::string &path);

  void input(GLuint idx, const std::string &name);
};
} // namespace cgl
