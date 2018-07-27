#pragma once

#include <stack>

#include <Eigen/Geometry>

class TransformStack {
  std::stack<Eigen::Projective3f> m_stack;

public:
  inline TransformStack() { clear(); }

  TransformStack &clear();

  TransformStack &ident();

  TransformStack &save();

  TransformStack &restore();

  template <typename T>
  TransformStack &operator<<(const T &rhs) {
    m_stack.top() = m_stack.top() * rhs;

    return *this;
  }

  template <typename T>
  TransformStack &operator>>(const T &rhs) {
    m_stack.top() = rhs * m_stack.top();

    return *this;
  }

  inline Eigen::Projective3f *      operator->() { return &m_stack.top(); }
  inline const Eigen::Projective3f *operator->() const {
    return &m_stack.top();
  }
  inline Eigen::Projective3f &      operator*() { return m_stack.top(); }
  inline const Eigen::Projective3f &operator*() const { return m_stack.top(); }
};
