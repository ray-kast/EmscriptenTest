#pragma once

#include <GLES2/gl2.h>

#undef Success // see program.cpp

#include <Eigen/Eigen>

namespace cgl {
class Uniform {
  GLint m_id;

  Uniform(GLint id) : m_id(id) {}

public:
  Uniform() : m_id(0) {}

  Uniform(const Uniform &) = delete;

  Uniform(Uniform &&) = default;

  void set(int);
  void set(float);

  void set(const Eigen::Vector2f &);
  void set(const Eigen::Vector3f &);
  void set(const Eigen::Vector4f &);

  void set(const Eigen::Vector2i &);
  void set(const Eigen::Vector3i &);
  void set(const Eigen::Vector4i &);

  void set(const Eigen::Matrix2f &, bool);

  void set(const Eigen::Matrix3f &, bool);

  void set(const Eigen::Matrix4f &, bool);

  inline void set(const Eigen::Affine2f &a, bool t) { set(a.matrix(), t); }

  inline void set(const Eigen::Affine3f &a, bool t) { set(a.matrix(), t); }

  inline void set(const Eigen::Projective2f &a, bool t) { set(a.matrix(), t); }

  inline void set(const Eigen::Projective3f &a, bool t) { set(a.matrix(), t); }

  friend class UseProgram;
};
} // namespace cgl
