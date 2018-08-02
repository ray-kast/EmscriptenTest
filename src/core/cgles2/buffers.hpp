#pragma once

#include <unordered_map>

#include "glad.hpp"

#include <linear.hpp>

#include "buffer.hpp"

namespace cgl {
class Buffers {
  GLsizei                             m_size;
  core::Linear<GLuint *, nullptr>     m_bufs;
  std::unordered_map<GLsizei, Buffer> m_owned;

public:
  constexpr const auto &size() const { return m_size; }

  Buffers() : m_size(0) {}

  Buffers(GLsizei);

  ~Buffers();

  Buffers &operator=(Buffers &&) = default;

  const Buffer &operator[](GLsizei);
};
} // namespace cgl
