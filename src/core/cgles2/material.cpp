#include "material.hpp"

#include <fileUtils.hpp>

#include <diag.hpp>

namespace cgl {
SetupMaterial::SetupMaterial(Material &mat) : m_mat(&mat) {
  mat.m_pgm = Program::create();
}

SetupMaterial::~SetupMaterial() {
  for (auto it = m_shaders.begin(), end = m_shaders.end(); it != end; ++it) {
    it->second.first.source(readFile(getExeDir() + it->second.second));
    it->second.first.compile();
  }

  m_mat->m_pgm.link();

#if !defined(_NDEBUG)
  m_mat->m_pgm.validate();
#endif

  m_mat->m_valid = true;
}

void SetupMaterial::add(GLenum type, const std::string &path) {
  auto pair = m_shaders.emplace(type, std::make_pair(type, path));

  if (!pair.second) die("shader type " + glEnumName(type) + " already added");

  m_mat->m_pgm.add(pair.first->second.first);
}

void SetupMaterial::input(GLuint idx, const std::string &name) {
  m_mat->m_pgm.semantic(idx, name);
}
} // namespace cgl
