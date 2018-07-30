#pragma once

#include <vector>

#include "buffers.hpp"

namespace cgl {
class Model {
  struct Attrib {
    const Buffer *buf;
    GLuint        idx;
    GLint         size;
    GLenum        type;
    GLsizei       stride;
    void *        offs;
  };

  GLenum              m_mode;
  Buffers             m_bufs;
  std::vector<Attrib> m_attribs;
  int                 m_curr = 0;
  const Buffer *      m_ibuf = nullptr;
  GLsizei             m_ibufLen;
  GLenum              m_ibufType;
  void *              m_ibufOffs;

public:
  Model() {}

  Model(GLenum mode, GLsizei size) : m_mode(mode), m_bufs(size) {}

  void loadVbuf(const void *data,
                std::size_t len,
                DataFreq    freq,
                DataAccess  access,
                GLuint      idx,
                GLint       size,
                GLenum      type,
                GLsizei     stride,
                void *      offs);

  void loadIbuf(const void *data,
                std::size_t len,
                DataFreq    freq,
                DataAccess  access,
                GLint       size,
                GLenum      type,
                void *      offs);

  // TODO: add vector and pointer overloads of loadVbuf and loadIbuf

  Model &operator=(Model &&) = default;

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
