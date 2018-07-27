#pragma once

#include "eigen/frustum.hpp"
#include "eigen/ortho.hpp"
#include "eigen/track.hpp"

namespace std {
template <typename Derived>
string to_string(const Eigen::EigenBase<Derived> &base) {
  ostringstream oss;
  oss << base.derived();
  return oss.str();
}
} // namespace std
