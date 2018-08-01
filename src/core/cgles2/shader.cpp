#include "shader.hpp"

#include <vector>

#include <diag.hpp>
#include <getPtr.hpp>

namespace cgl {
std::string Shader::getLog() {
  GLint len;
  glGetShaderiv(m_shd, GL_INFO_LOG_LENGTH, &len);

  std::vector<char> log(len);

  glGetShaderInfoLog(m_shd, len, &len, _getVecPtr(log));

  return std::string(log.begin(), log.end());
}

Shader::Shader(GLenum type) {
  m_shd = glCreateShader(type);
  if (!m_shd) die("glCreateShader failed");
}

Shader::~Shader() {
  if (m_shd.empty()) return;
  glDeleteShader(m_shd);
}

void Shader::source(const std::string &str) {
  const char *buf = str.c_str();
  int         len = str.size();
  glShaderSource(m_shd, 1, &buf, &len);
}

void Shader::compile() {
  glCompileShader(m_shd);

  GLint success;
  glGetShaderiv(m_shd, GL_COMPILE_STATUS, &success);

  if (success) {
    if (auto log = getLog(); !log.empty())
      warn("in shader " + std::to_string(m_shd) + ": " + log);
  }
  else {
    err("in shader " + std::to_string(m_shd) + ": " + getLog());
    die("glCompileShader failed");
  }
}
} // namespace cgl
