#include "texture.hpp"

#include <unordered_map>
#include <unordered_set>

#include <diag.hpp>

namespace cgl {
static std::unordered_map<GLenum, std::unordered_set<GLenum>> s_bound;

BindTexture::BindTexture(GLenum unit, GLenum target, const Texture &tex) :
    m_target(target),
    m_tex(&tex),
    m_unit(unit) {
  if (!s_bound[m_unit].emplace(m_target).second)
    die("texture type " + std::to_string(target) + " already bound in unit " +
        std::to_string(m_unit) + ".");

  glActiveTexture(GL_TEXTURE0 + m_unit);
  glBindTexture(target, tex.m_tex);
}

BindTexture::~BindTexture() {
  if (m_target.empty()) return;
  glActiveTexture(m_unit);
  glBindTexture(m_target, 0);
  s_bound.at(m_unit).erase(m_target);
}

void BindTexture::image(GLint       lvl,
                        GLint       internalFormat,
                        GLsizei     width,
                        GLsizei     height,
                        GLint       border,
                        GLenum      format,
                        GLenum      type,
                        const void *data) {
  glTexImage2D(
      m_target, lvl, internalFormat, width, height, border, format, type, data);
}
} // namespace cgl
