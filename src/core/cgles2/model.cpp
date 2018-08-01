#include "model.hpp"

#include <cassert>

#include <diag.hpp>

namespace cgl {
BindBuffer Model::addVbuf(std::size_t bufId,
                          GLuint      idx,
                          GLint       size,
                          GLenum      type,
                          GLsizei     stride,
                          void *      offs) {
  auto &&buf = m_bufs[bufId];

  if (!m_attribs
           .emplace(idx,
                    Attrib{.buf    = &buf,
                           .size   = size,
                           .type   = type,
                           .stride = stride,
                           .offs   = offs})
           .second)
    die("attribute array " + std::to_string(idx) + " already bound");

  return BindBuffer(GL_ARRAY_BUFFER, buf);
}

BindBuffer Model::addIbuf(std::size_t bufId, GLenum type, void *offs) {
  auto &&buf = m_bufs[bufId];

  if (m_ibuf) die("index buffer already set");

  m_ibuf     = &buf;
  m_ibufType = type;
  m_ibufOffs = offs;

  return BindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf);
}

BindBuffer Model::bindVbuf(GLuint idx) const {
  return BindBuffer(GL_ARRAY_BUFFER, *m_attribs.at(idx).buf);
}

BindBuffer Model::bindIbuf() const {
  return BindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_ibuf);
}

SelectModel::SelectModel(const Model &model) :
    m_model(&model),
    m_ibo(GL_ELEMENT_ARRAY_BUFFER, *model.m_ibuf) {
  for (auto &[idx, attrib] : model.m_attribs) {
    BindBuffer vbo(GL_ARRAY_BUFFER, *attrib.buf);

    glVertexAttribPointer(
        idx, attrib.size, attrib.type, false, attrib.stride, attrib.offs);

    glEnableVertexAttribArray(idx);
  }
}

SelectModel::~SelectModel() {
  if (m_model.empty()) return;

  for (auto &[idx, _] : m_model.get()->m_attribs)
    glDisableVertexAttribArray(idx);
}

void SelectModel::draw() {
  auto &&mdl = m_model.get();
  glDrawElements(mdl->m_mode, mdl->m_ibufLen, mdl->m_ibufType, mdl->m_ibufOffs);
}
} // namespace cgl
