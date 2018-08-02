#pragma once

#include <unordered_map>

#include "glad.hpp"

#include <linear.hpp>

#include "renderbuffer.hpp"

namespace cgl {
class Renderbuffers {
  GLsizei                                   m_size;
  core::Linear<GLuint *, nullptr>           m_rbufs;
  std::unordered_map<GLsizei, Renderbuffer> m_owned;

public:
  constexpr const auto &size() const { return m_size; }

  Renderbuffers() : m_size(0) {}

  Renderbuffers(GLsizei);

  ~Renderbuffers();

  Renderbuffers &operator=(Renderbuffers &&) = default;

  const Renderbuffer &operator[](GLsizei);
};
} // namespace cgl
