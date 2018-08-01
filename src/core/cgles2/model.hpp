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

  GLenum                        m_mode;
  Buffers                       m_bufs;
  std::map<std::string, Attrib> m_attribs;
  const Buffer *                m_ibuf    = nullptr;
  GLsizei                       m_ibufLen = 0;
  GLenum                        m_ibufType;
  void *                        m_ibufOffs;

public:
  Model() {}

  Model(GLenum mode, GLsizei size) : m_mode(mode), m_bufs(size) {}

  BindBuffer addVbuf(std::size_t bufId,
                     const std::string &,
                     GLint   size,
                     GLenum  type,
                     GLsizei stride,
                     void *  offs);

  BindBuffer addIbuf(std::size_t bufId, GLenum type, void *offs);

  BindBuffer bindVbuf(const std::string &) const;

  BindBuffer bindIbuf() const;

  void ibufLen(GLsizei len) { m_ibufLen = len; }

  Model &operator=(Model &&) = default;

  friend class SelectModel;
};

class SelectModel {
  core::Linear<const Model *, nullptr> m_model;
  BindBuffer                           m_ibo;
  std::vector<GLuint>                  m_attribs;
  GLuint                               m_pgm;

public:
  SelectModel() {}

  explicit SelectModel(const Model &);

  ~SelectModel();

  void draw();
};
} // namespace cgl
