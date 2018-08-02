#pragma once

#include "glad.hpp"

#include <linear.hpp>

namespace cgl {
class Framebuffer {
  core::Linear<GLuint, 0> m_fbuf;

  Framebuffer(GLuint fbuf) : m_fbuf(fbuf) {}

public:
  Framebuffer() {}

  friend class Framebuffers;
  friend class BindFramebuffer;
};

class BindTexture;
class BindRenderbuffer;

class BindFramebuffer {
  core::Linear<GLenum, 0> m_target;

public:
  BindFramebuffer() {}

  BindFramebuffer(GLenum, const Framebuffer &);

  ~BindFramebuffer();

  void texture2D(GLenum attachment, const BindTexture &, GLint lvl);

  void renderBuf(GLenum attachment, const BindRenderbuffer &);

  void assertStatus();

  BindFramebuffer &operator=(BindFramebuffer &&) = default;
};
} // namespace cgl
