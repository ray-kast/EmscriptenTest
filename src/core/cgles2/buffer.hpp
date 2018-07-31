#pragma once

#include <GLES2/gl2.h>

#include <getPtr.hpp>
#include <linear.hpp>

namespace cgl {
enum DataFreq {
  FreqStream,
  FreqStatic,
  FreqDynamic,
};

enum DataAccess {
  AccessDraw,
};

GLenum getDataUse(DataFreq, DataAccess);

class Buffer {
  core::Linear<GLuint, 0> m_buf;

  Buffer(GLuint buf) : m_buf(buf) {}

public:
  Buffer() {}

  friend class Buffers;
  friend class BindBuffer;
};

class BindBuffer {
  core::Linear<GLenum, 0> m_target;

public:
  BindBuffer() {}

  BindBuffer(GLenum, const Buffer &);

  ~BindBuffer();

  void data(const void *, std::size_t, DataFreq, DataAccess = AccessDraw);

  template <typename T>
  void data(const std::vector<T> &vec,
            DataFreq              freq,
            DataAccess            access = AccessDraw) {
    data(_getVecPtr(vec), sizeof(T) * vec.size(), freq, access);
  }

  template <typename T>
  void data(const T &obj, DataFreq freq, DataAccess access = AccessDraw) {
    data(_getPtr(obj), sizeof(obj), freq, access);
  }

  BindBuffer &operator=(BindBuffer &&) = default;
};
} // namespace cgl
