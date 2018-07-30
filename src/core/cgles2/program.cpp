#include "program.hpp"

#include <vector>

#include <diag.hpp>

#include "shader.hpp"

namespace cgl {
std::string Program::getLog() {
  GLint len;
  glGetProgramiv(m_pgm, GL_INFO_LOG_LENGTH, &len);

  std::vector<char> log(len);

  glGetProgramInfoLog(m_pgm, len, &len, &log[0]);

  return std::string(log.begin(), log.end());
}

Program Program::create() {
  auto pgm = glCreateProgram();
  if (!pgm) die("glCreateProgram failed");
  return Program(pgm);
}

Program::~Program() {
  if (m_pgm.empty()) return;

  for (auto &attached : m_attached) glDetachShader(m_pgm, attached);

  glDeleteProgram(m_pgm);
}

// TODO: do I need to check glGetError, or do I just use glad?
void Program::add(const Shader &shd) {
  m_attached.emplace(shd.m_shd);
  glAttachShader(m_pgm, shd.m_shd);
}

void Program::semantic(GLuint idx, const std::string &name) {
  if (auto it = m_semantics.find(idx); it != m_semantics.end())
    die("program " + std::to_string(m_pgm) + " input index " +
        std::to_string(idx) + " already bound to '" + it->second + "'");

  m_semantics.emplace(idx, name);
  glBindAttribLocation(m_pgm, idx, name.c_str());
}

void Program::link() {
  glLinkProgram(m_pgm);

  GLint success;
  glGetProgramiv(m_pgm, GL_LINK_STATUS, &success);

  if (!success) {
    err("in program " + std::to_string(m_pgm) + ": " + getLog());
    die("glLinkProgram failed");
  }

  for (auto &attached : m_attached) glDetachShader(m_pgm, attached);

  m_attached.clear();
}

void Program::validate() {
  glValidateProgram(m_pgm);

  GLint success;
  glGetProgramiv(m_pgm, GL_VALIDATE_STATUS, &success);

  if (!success) {
    err("validating program " + std::to_string(m_pgm) + ": " + getLog());
    die("glValidateProgram failed");
  }
}

static GLuint s_using = false;

UseProgram::UseProgram(const Program &pgm) : m_pgm(&pgm) {
  if (s_using) die("program already in use");
  s_using = true;

  glUseProgram(m_pgm.get()->m_pgm);
}

UseProgram::~UseProgram() {
  if (m_pgm.empty()) return;
  glUseProgram(0);
  s_using = false;
}

Uniform &UseProgram::uniform(const std::string &name) {
  auto it = m_uniforms.find(name);

  if (it == m_uniforms.end()) {
    int id = glGetUniformLocation(m_pgm.get()->m_pgm, name.c_str());

    if (id < 0) warn("bad uniform name '" + name + "'");

    it = m_uniforms.emplace(name, Uniform(id)).first;
  }

  return it->second;
}
} // namespace cgl
