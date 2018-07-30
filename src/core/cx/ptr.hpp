#pragma once

#include <X11/Xlib.h>

namespace cx {
template <typename T>
class Ptr {
  T *m_ptr;

public:
  constexpr auto &get() { return m_ptr; }
  constexpr auto &get() const { return m_ptr; }

  Ptr(T *ptr) : m_ptr(ptr) {}

  Ptr() : Ptr(nullptr) {}

  ~Ptr() { XFree(m_ptr); }

  auto *operator->() const { return m_ptr; }
  auto &operator*() const { return *m_ptr; }

  constexpr operator T *&() { return m_ptr; }
  constexpr operator T *&() const { return m_ptr; }
};
} // namespace cx
