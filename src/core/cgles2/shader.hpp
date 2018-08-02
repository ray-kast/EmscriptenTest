#pragma once

#include "glad.hpp"

#include <linear.hpp>

namespace cgl {
class Shader {
  core::Linear<GLuint, 0> m_shd;

  std::string getLog();

public:
  Shader() {}

  explicit Shader(GLenum type);

  ~Shader();

  void source(const std::string &);

  void compile();

  Shader &operator=(Shader &&) = default;

  friend class Program;
};
} // namespace cgl
