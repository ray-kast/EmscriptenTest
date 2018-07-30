#pragma once

#include <map>
#include <vector>

#include "buffers.hpp"

namespace cgl {
class Model {
  struct Attrib {
    const Buffer *buf;
    GLint         size;
    GLenum        type;
    GLsizei       stride;
    void *        offs;
  };


  GLenum                   m_mode;
  Buffers                  m_bufs;
  std::map<GLuint, Attrib> m_attribs;
  const Buffer *           m_ibuf    = nullptr;
  GLsizei                  m_ibufLen = 0;
  GLenum                   m_ibufType;
  void *                   m_ibufOffs;

public:
  Model() {}

  Model(GLenum mode, GLsizei size) : m_mode(mode), m_bufs(size) {}

  const Buffer &addVbuf(std::size_t bufId,
                        GLuint      idx,
                        GLint       size,
                        GLenum      type,
                        GLsizei     stride,
                        void *      offs);

  const Buffer &addIbuf(std::size_t bufId, GLenum type, void *offs);

  void ibufLen(GLsizei len) { m_ibufLen = len; }

  Model &operator=(Model &&) = default;

  const Buffer &operator[](std::size_t i) { return m_bufs[i]; }

  friend class SelectModel;
};

// TODO: this isn't linear
class SelectModel {
  core::Linear<const Model *, nullptr> m_model;
  BindBuffer                           m_ibo;
  std::vector<GLuint>                  m_attribs;

public:
  SelectModel() {}

  explicit SelectModel(const Model &);

  ~SelectModel();

  void draw();
};
} // namespace cgl
