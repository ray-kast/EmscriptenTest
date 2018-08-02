#pragma once

#include <unordered_map>

#include "glad.hpp"

#include <linear.hpp>

#include "framebuffer.hpp"

namespace cgl {
class Framebuffers {
  GLsizei                                  m_size;
  core::Linear<GLuint *, nullptr>          m_fbufs;
  std::unordered_map<GLsizei, Framebuffer> m_owned;

public:
  constexpr const auto &size() const { return m_size; }

  Framebuffers() : m_size(0) {}

  Framebuffers(GLsizei);

  ~Framebuffers();

  Framebuffers &operator=(Framebuffers &&) = default;

  const Framebuffer &operator[](GLsizei);
};
} // namespace cgl
