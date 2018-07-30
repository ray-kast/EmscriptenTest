#include "uniform.hpp"

namespace cgl {
void Uniform::set(int val) { glUniform1i(m_id, val); }

void Uniform::set(float val) { glUniform1f(m_id, val); }

void Uniform::set(const Eigen::Vector2f &vec) {
  glUniform2fv(m_id, 1, vec.data());
}

void Uniform::set(const Eigen::Vector3f &vec) {
  glUniform3fv(m_id, 1, vec.data());
}

void Uniform::set(const Eigen::Vector4f &vec) {
  glUniform4fv(m_id, 1, vec.data());
}

void Uniform::set(const Eigen::Vector2i &vec) {
  glUniform2iv(m_id, 1, vec.data());
}

void Uniform::set(const Eigen::Vector3i &vec) {
  glUniform3iv(m_id, 1, vec.data());
}

void Uniform::set(const Eigen::Vector4i &vec) {
  glUniform4iv(m_id, 1, vec.data());
}

void Uniform::set(const Eigen::Matrix2f &mat, bool transp) {
  glUniformMatrix2fv(m_id, 1, transp, mat.data());
}

void Uniform::set(const Eigen::Matrix3f &mat, bool transp) {
  glUniformMatrix3fv(m_id, 1, transp, mat.data());
}

void Uniform::set(const Eigen::Matrix4f &mat, bool transp) {
  glUniformMatrix4fv(m_id, 1, transp, mat.data());
}
} // namespace cgl
