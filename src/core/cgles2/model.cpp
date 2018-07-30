#include "model.hpp"

#include <cassert>

#include <diag.hpp>

namespace cgl {
void Model::loadVbuf(const void *data,
                     std::size_t len,
                     DataFreq    freq,
                     DataAccess  access,
                     GLuint      idx,
                     GLint       size,
                     GLenum      type,
                     GLsizei     stride,
                     void *      offs) {
  assert(m_curr < m_bufs.size());

  {
    BindBuffer vbo(GL_ARRAY_BUFFER, m_bufs[m_curr]);
    vbo.data(data, len, freq, access);
  }

  m_attribs.push_back(Attrib{.buf    = &m_bufs[m_curr],
                             .idx    = idx,
                             .size   = size,
                             .type   = type,
                             .stride = stride,
                             .offs   = offs});

  ++m_curr;
}

void Model::loadIbuf(const void *data,
                     std::size_t len,
                     DataFreq    freq,
                     DataAccess  access,
                     GLint       size,
                     GLenum      type,
                     void *      offs) {
  assert(m_curr < m_bufs.size());

  {
    BindBuffer ibo(GL_ELEMENT_ARRAY_BUFFER, m_bufs[m_curr]);
    ibo.data(data, len, freq, access);
  }

  m_ibuf     = &m_bufs[m_curr];
  m_ibufLen  = len / size;
  m_ibufType = type;
  m_ibufOffs = offs;

  ++m_curr;
}

static bool s_selected = false;

SelectModel::SelectModel(const Model &model) :
    m_model(&model),
    m_ibo(GL_ELEMENT_ARRAY_BUFFER, *model.m_ibuf) {
  if (s_selected) die("model already selected");

  s_selected = true;

  for (auto &attrib : model.m_attribs) {
    BindBuffer vbo(GL_ARRAY_BUFFER, *attrib.buf);

    glVertexAttribPointer(attrib.idx,
                          attrib.size,
                          attrib.type,
                          false,
                          attrib.stride,
                          attrib.offs);

    glEnableVertexAttribArray(attrib.idx);
  }
}

SelectModel::~SelectModel() {
  if (m_model.empty()) return;

  for (auto &attrib : m_model.get()->m_attribs) {
    glDisableVertexAttribArray(attrib.idx);
  }

  s_selected = false;
}

void SelectModel::draw() {
  auto &&mdl = m_model.get();
  glDrawElements(mdl->m_mode, mdl->m_ibufLen, mdl->m_ibufType, mdl->m_ibufOffs);
}
} // namespace cgl
