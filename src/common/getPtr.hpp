#pragma once

#include <type_traits>

template <typename T>
constexpr std::enable_if_t<std::is_array<T>::value, const T &> _getPtr(
    const T &obj) {
  return obj;
}

template <typename T>
constexpr std::enable_if_t<!std::is_array<T>::value, const T *> _getPtr(
    const T &obj) {
  return &obj;
}
