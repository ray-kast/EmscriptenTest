#pragma once

#include <Eigen/Eigen>

namespace Eigen {
template <typename _Scalar>
class Ortho3 {
  static const int _Dim = 3;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(_Scalar, _Dim);

  enum { Dim = _Dim };

  typedef _Scalar Scalar;

  typedef Matrix<Scalar, 2, 1> Vector2;
  typedef Matrix<Scalar, 3, 1> Vector3;
  typedef Matrix<Scalar, 4, 4> Matrix4;

  typedef Matrix<Scalar, Dim, 1> VectorType;

  typedef Transform<Scalar, Dim, Projective> ProjectiveTransformType;

protected:
  Vector3 m_min, m_max;

public:
  EIGEN_DEVICE_FUNC Ortho3() {}

  EIGEN_DEVICE_FUNC inline Ortho3(const Vector2 &min,
                                  const Vector2 &max,
                                  const Scalar & zNear,
                                  const Scalar & zFar) {
    m_min.template head<2>() = min;
    m_min.z()                = zNear;

    m_max.template head<2>() = max;
    m_max.z()                = zFar;
  }

  EIGEN_DEVICE_FUNC inline Ortho3(const Vector2 &size,
                                  const Scalar & zNear,
                                  const Scalar & zFar) {
    m_min.template head<2>() = size * Scalar(-.5);
    m_min.z()                = zNear;

    m_max.template head<2>() = size * Scalar(.5);
    m_max.z()                = zFar;
  }

  EIGEN_DEVICE_FUNC inline Scalar xMin() const { return m_min.x(); }
  EIGEN_DEVICE_FUNC inline Scalar yMin() const { return m_min.y(); }
  EIGEN_DEVICE_FUNC inline Scalar zMin() const { return m_min.z(); }
  EIGEN_DEVICE_FUNC inline Scalar xMax() const { return m_max.x(); }
  EIGEN_DEVICE_FUNC inline Scalar yMax() const { return m_max.y(); }
  EIGEN_DEVICE_FUNC inline Scalar zMax() const { return m_max.z(); }

  EIGEN_DEVICE_FUNC inline Scalar &xMin() { return m_min.x(); }
  EIGEN_DEVICE_FUNC inline Scalar &yMin() { return m_min.y(); }
  EIGEN_DEVICE_FUNC inline Scalar &zMin() { return m_min.z(); }
  EIGEN_DEVICE_FUNC inline Scalar &xMax() { return m_max.x(); }
  EIGEN_DEVICE_FUNC inline Scalar &yMax() { return m_max.y(); }
  EIGEN_DEVICE_FUNC inline Scalar &zMax() { return m_max.z(); }

  EIGEN_DEVICE_FUNC const Vector3 &min() const { return m_min; }
  EIGEN_DEVICE_FUNC Vector3 &min() { return m_min; }

  EIGEN_DEVICE_FUNC const Vector3 &max() const { return m_max; }
  EIGEN_DEVICE_FUNC Vector3 &max() { return m_max; }

  EIGEN_DEVICE_FUNC Matrix4 matrix() const {
    Matrix4 res;
    res.setZero();

    Vector3 size = m_max - m_min;

    res.col(3).template head<3>() = -(m_min + m_max).cwiseQuotient(size);
    res(0, 0)                     = Scalar(2) / size.x();
    res(1, 1)                     = Scalar(2) / size.y();
    res(2, 2)                     = Scalar(-2) / size.z();
    res(3, 3)                     = Scalar(1);

    return res;
  }

  template <int Mode, int Options>
  EIGEN_DEVICE_FUNC inline Transform<Scalar, Dim, Mode> operator*(
      const Transform<Scalar, Dim, Mode, Options> &t) const {
    Transform<Scalar, Dim, Mode> res = t;
    res.matrix().applyOnTheLeft(matrix());
    return res;
  }

  EIGEN_DEVICE_FUNC inline ProjectiveTransformType operator*(
      const Translation<Scalar, Dim> &other) const {
    return *this * ProjectiveTransformType(other);
  }

  EIGEN_DEVICE_FUNC inline ProjectiveTransformType operator*(
      const UniformScaling<Scalar> &other) const {
    return *this * ProjectiveTransformType(other);
  }

  template <typename OtherDerived>
  EIGEN_DEVICE_FUNC inline ProjectiveTransformType operator*(
      const EigenBase<OtherDerived> &linear) const {
    return *this * ProjectiveTransformType(linear);
  }

  template <typename Derived>
  EIGEN_DEVICE_FUNC inline ProjectiveTransformType operator*(
      const RotationBase<Derived, Dim> &r) const {
    return *this * ProjectiveTransformType(r);
  }

  template <typename Derived>
  inline typename internal::enable_if<Derived::IsVectorAtCompileTime,
                                      VectorType>::type
  operator*(const MatrixBase<Derived> &vec) const {
    return (matrix() * vec.derived().homogeneous()).hnormalized();
  }

  template <int Mode, int Options>
  friend EIGEN_DEVICE_FUNC inline Transform<Scalar, Dim, Mode> operator*(
      const Transform<Scalar, Dim, Mode, Options> &t, const Ortho3 &f) {
    Transform<Scalar, Dim, Mode> res = t;
    res.matrix().applyOnTheRight(f.matrix());
    return res;
  }

  friend EIGEN_DEVICE_FUNC inline ProjectiveTransformType operator*(
      const Translation<Scalar, 3> &other, const Ortho3 &f) {
    return ProjectiveTransformType(other) * f;
  }

  friend EIGEN_DEVICE_FUNC inline ProjectiveTransformType operator*(
      const UniformScaling<Scalar> &other, const Ortho3 &f) {
    return ProjectiveTransformType(other) * f;
  }

  template <typename OtherDerived>
  friend EIGEN_DEVICE_FUNC inline ProjectiveTransformType operator*(
      const EigenBase<OtherDerived> &linear, const Ortho3 &f) {
    return ProjectiveTransformType(linear) * f;
  }

  template <typename Derived>
  friend EIGEN_DEVICE_FUNC inline ProjectiveTransformType operator*(
      const RotationBase<Derived, 3> &r, const Ortho3 &f) {
    return ProjectiveTransformType(r) * f;
  }

  template <typename NewScalarType>
  EIGEN_DEVICE_FUNC inline
      typename internal::cast_return_type<Ortho3, Ortho3<NewScalarType>>::type
      cast() const {
    return
        typename internal::cast_return_type<Ortho3,
                                            Ortho3<NewScalarType>>::type(*this);
  }

  EIGEN_DEVICE_FUNC inline operator ProjectiveTransformType() const {
    ProjectiveTransformType res;
    res.matrix() = matrix();
    return res;
  }

  template <typename OtherScalarType>
  EIGEN_DEVICE_FUNC inline explicit Ortho3(
      const Ortho3<OtherScalarType> &other) {
    m_min = other.min().template cast<Scalar>();
    m_max = other.max().template cast<Scalar>();
  }

  EIGEN_DEVICE_FUNC bool isApprox(
      const Ortho3 &                   other,
      typename NumTraits<Scalar>::Real prec =
          NumTraits<Scalar>::dummy_precision()) const {
    return m_min.isApprox(other.m_min, prec) &&
           m_max.isApprox(other.m_max, prec);
  }
};

typedef Ortho3<float>  Ortho3f;
typedef Ortho3<double> Ortho3d;
} // namespace Eigen
