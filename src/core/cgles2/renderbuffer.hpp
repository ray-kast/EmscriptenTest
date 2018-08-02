#pragma once

#include "glad.hpp"

#include <linear.hpp>

namespace cgl {
class Renderbuffer {
  core::Linear<GLuint, 0> m_rbuf;

  Renderbuffer(GLuint rbuf) : m_rbuf(rbuf) {}

public:
  Renderbuffer() {}

  friend class Renderbuffers;
  friend class BindRenderbuffer;
  friend class BindFramebuffer;
};

class BindTexture;

class BindRenderbuffer {
  core::Linear<GLenum, 0> m_target;
  const Renderbuffer *    m_rbuf;

public:
  BindRenderbuffer() {}

  BindRenderbuffer(GLenum, const Renderbuffer &);

  ~BindRenderbuffer();

  void storage(GLenum format, GLsizei width, GLsizei height);

  BindRenderbuffer &operator=(BindRenderbuffer &&) = default;

  friend class BindFramebuffer;
};
} // namespace cgl
