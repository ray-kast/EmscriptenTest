#pragma once

#include <functional>

namespace core {
template <typename T, T Default>
class Linear {
  T m_value;

public:
  constexpr auto &      get() { return m_value; }
  constexpr const auto &get() const { return m_value; }

  bool empty() const { return m_value == Default; }

  Linear() : m_value(Default) {}

  Linear(Linear &)       = delete;
  Linear(const Linear &) = delete;
  Linear(Linear &&rhs) : m_value(std::move(rhs.m_value)) {
    rhs.m_value = Default;
  }

  template <typename... TArgs>
  Linear(TArgs &&... args) : m_value(std::forward<TArgs>(args)...) {}

  operator T &() { return m_value; }
  operator const T &() const { return m_value; }

  Linear &operator=(Linear &) = delete;
  Linear &operator=(const Linear &) = delete;

  Linear &operator=(Linear &&rhs) {
    if (this == &rhs) return *this;
    m_value     = std::move(rhs.m_value);
    rhs.m_value = Default;
    return *this;
  }
};
} // namespace core
