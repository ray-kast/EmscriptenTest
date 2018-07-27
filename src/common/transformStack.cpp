#include "transformStack.hpp"

TransformStack &TransformStack::clear() {
  m_stack = decltype(m_stack){};
  m_stack.push(Eigen::Projective3f::Identity());
  return *this;
}

TransformStack &TransformStack::ident() {
  m_stack.top().setIdentity();
  return *this;
}

TransformStack &TransformStack::save() {
  m_stack.push(m_stack.top());
  return *this;
}

TransformStack &TransformStack::restore() {
  m_stack.pop();
  return *this;
}
