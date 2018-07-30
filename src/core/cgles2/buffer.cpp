#include "buffer.hpp"

#include <string>
#include <unordered_set>

#include <diag.hpp>

namespace cgl {
GLenum getDataUse(DataFreq freq, DataAccess access) {
  switch (freq) {
  case FreqStream:
    switch (access) {
    case AccessDraw: return GL_STREAM_DRAW;
    }
  case FreqStatic:
    switch (access) {
    case AccessDraw: return GL_STATIC_DRAW;
    }
  case FreqDynamic:
    switch (access) {
    case AccessDraw: return GL_DYNAMIC_DRAW;
    }
  }

  die("invalid arguments to getDataUse");
}

static std::unordered_set<GLenum> s_bound;

BindBuffer::BindBuffer(GLenum target, const Buffer &buf) : m_target(target) {
  if (!s_bound.emplace(m_target).second)
    die("buffer type " + std::to_string(target) + " already bound");

  glBindBuffer(target, buf.m_buf);
}

BindBuffer::~BindBuffer() {
  if (m_target.empty()) return;
  glBindBuffer(m_target, 0);
  s_bound.erase(m_target);
}

void BindBuffer::data(const void *data,
                      std::size_t size,
                      DataFreq    freq,
                      DataAccess  access) {
  glBufferData(m_target, size, data, getDataUse(freq, access));
}
} // namespace cgl
