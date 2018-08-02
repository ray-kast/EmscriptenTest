#include "texture.hpp"

#include <unordered_map>
#include <unordered_set>

#include <SDL/SDL_image.h>

#include <fileUtils.hpp>
#include <getPtr.hpp>
#include <matrixMath.hpp>

#include <diag.hpp>

namespace cgl {
static std::unordered_map<GLenum, std::unordered_set<GLenum>> s_bound;

void BindTexture::activate() { glActiveTexture(GL_TEXTURE0 + m_unit); }

BindTexture::BindTexture(GLenum unit, GLenum target, const Texture &tex) :
    m_target(target),
    m_tex(&tex),
    m_unit(unit) {
  if (!s_bound[m_unit].emplace(m_target).second)
    die("texture type " + std::to_string(target) + " already bound in unit " +
        std::to_string(m_unit) + ".");

  activate();
  glBindTexture(target, tex.m_tex);
}

BindTexture::~BindTexture() {
  if (m_target.empty()) return;
  activate();
  glBindTexture(m_target, 0);
  s_bound.at(m_unit).erase(m_target);
}

void BindTexture::param(GLenum name, int val) {
  glTexParameteri(m_target, name, val);
}

void BindTexture::param(GLenum name, const int *val) {
  glTexParameteriv(m_target, name, val);
}

void BindTexture::param(GLenum name, float val) {
  glTexParameterf(m_target, name, val);
}

void BindTexture::param(GLenum name, const float *val) {
  glTexParameterfv(m_target, name, val);
}

void BindTexture::image(GLint       lvl,
                        GLsizei     width,
                        GLsizei     height,
                        GLenum      format,
                        GLenum      type,
                        const void *data) {
  activate();
  glTexImage2D(m_target, lvl, format, width, height, 0, format, type, data);
}

// TODO: Write SDL wrapper code
void BindTexture::loadImage(GLint lvl, const std::string &path) {
  std::string realpath = getExeDir() + path;
  auto        isurf    = IMG_Load(realpath.c_str());

  if (!isurf) die("IMG_Load failed for path '" + path + "'");

  if (SDL_LockSurface(isurf)) die("SDL_LockSurface failed");

  GLenum format;

  switch (isurf->format->BytesPerPixel) {
  case 1: format = GL_LUMINANCE; break;
  case 2: format = GL_LUMINANCE_ALPHA; break;
  case 3: format = GL_RGB; break;
  case 4: format = GL_RGBA; break;
  default:
    die("bad byte-per-pixel value " +
        std::to_string(isurf->format->BytesPerPixel) + " for image '" + path +
        "'");
  }

  image(lvl, isurf->w, isurf->h, format, GL_UNSIGNED_BYTE, isurf->pixels);
  genMipmap(); // TODO: this needs to be smarter

  SDL_UnlockSurface(isurf);
  SDL_FreeSurface(isurf);
}

void BindTexture::genMipmap() { glGenerateMipmap(m_target); }
} // namespace cgl
