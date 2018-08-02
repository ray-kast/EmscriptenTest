#pragma once

#include <unordered_map>
#include <unordered_set>

#include "glad.hpp"

#include <linear.hpp>

#include "uniform.hpp"

namespace cgl {
class Shader;

class Program {
  core::Linear<GLuint, 0>                 m_pgm;
  std::unordered_set<GLuint>              m_attached;
  std::unordered_map<GLuint, std::string> m_semantics;

  Program(GLuint pgm) : m_pgm(pgm) {}

  std::string getLog();

public:
  static Program create();

  const auto &semantic(GLuint idx) const { return m_semantics.at(idx); }

  Program() {}

  ~Program();

  void add(const Shader &);

  void semantic(GLuint idx, const std::string &name);

  void link();

  void validate();

  Program &operator=(Program &&) = default;

  friend class UseProgram;
};

class UseProgram {
  core::Linear<const Program *, nullptr>   m_pgm;
  std::unordered_map<std::string, Uniform> m_uniforms;

public:
  UseProgram() {}

  explicit UseProgram(const Program &);

  ~UseProgram();

  Uniform &uniform(const std::string &);

  UseProgram &operator=(UseProgram &&) = default;
};
} // namespace cgl
