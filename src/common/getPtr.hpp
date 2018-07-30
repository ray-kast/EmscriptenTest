#pragma once

#include <type_traits>
#include <vector>

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

template <typename T>
constexpr T *_getVecPtr(const std::vector<T> &vec) {
  return const_cast<T *>(vec.empty() ? nullptr : &vec[0]);
}
