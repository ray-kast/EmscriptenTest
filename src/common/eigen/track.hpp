#pragma once

#include <Eigen/Eigen>

#include <iostream>

namespace Eigen {
template <typename _Scalar>
class Track3 {
  static const int _Dim = 3;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF_VECTORIZABLE_FIXED_SIZE(_Scalar, _Dim);

  enum { Dim = _Dim };

  typedef _Scalar Scalar;

  typedef Matrix<Scalar, Dim, 1> VectorType;

  typedef Translation<Scalar, Dim> TranslationType;

  typedef AngleAxis<Scalar> AngleAxisType;

  typedef Transform<Scalar, Dim, Affine> AffineTransformType;

protected:
  VectorType m_fwd, m_up;
  bool       m_flipX;

public:
  EIGEN_DEVICE_FUNC Track3() {}

  EIGEN_DEVICE_FUNC inline Track3(const VectorType &fwd,
                                  const VectorType &up,
                                  bool              flipX) :
      m_fwd(fwd),
      m_up(up),
      m_flipX(flipX) {}

  EIGEN_DEVICE_FUNC const VectorType &fwd() const { return m_fwd; }
  EIGEN_DEVICE_FUNC VectorType &fwd() { return m_fwd; }

  EIGEN_DEVICE_FUNC const VectorType &up() const { return m_up; }
  EIGEN_DEVICE_FUNC VectorType &up() { return m_up; }

  EIGEN_DEVICE_FUNC const VectorType &flipX() const { return m_flipX; }
  EIGEN_DEVICE_FUNC VectorType &flipX() { return m_flipX; }

  EIGEN_DEVICE_FUNC AffineTransformType transform() const {
    AffineTransformType res;
    res.setIdentity();

    auto &mtx = res.matrix();

    mtx.row(0).template head<3>() = (m_flipX ? m_up.cross(m_fwd)
                                             : m_fwd.cross(m_up))
                                        .normalized();
    mtx.row(1).template head<3>() = (m_up - m_fwd * m_fwd.dot(m_up) /
                                                m_fwd.squaredNorm())
                                        .normalized();
    mtx.row(2).template head<3>() = m_fwd.normalized();

    return res;
  }

  template <int Mode, int Options>
  EIGEN_DEVICE_FUNC inline Transform<Scalar, Dim, Mode> operator*(
      const Transform<Scalar, Dim, Mode, Options> &t) const {
    return t * transform();
  }

  EIGEN_DEVICE_FUNC inline AffineTransformType operator*(
      const Translation<Scalar, Dim> &other) const {
    return *this * AffineTransformType(other);
  }

  EIGEN_DEVICE_FUNC inline AffineTransformType operator*(
      const UniformScaling<Scalar> &other) const {
    return *this * AffineTransformType(other);
  }

  template <typename OtherDerived>
  EIGEN_DEVICE_FUNC inline AffineTransformType operator*(
      const EigenBase<OtherDerived> &linear) const {
    return *this * AffineTransformType(linear);
  }

  template <typename Derived>
  EIGEN_DEVICE_FUNC inline AffineTransformType operator*(
      const RotationBase<Derived, Dim> &r) const {
    return *this * AffineTransformType(r);
  }

  template <typename Derived>
  inline typename internal::enable_if<Derived::IsVectorAtCompileTime,
                                      VectorType>::type
  operator*(const MatrixBase<Derived> &vec) const {
    return (transform() * vec.derived().homogeneous()).hnormalized();
  }

  template <int Mode, int Options>
  friend EIGEN_DEVICE_FUNC inline Transform<Scalar, Dim, Mode> operator*(
      const Transform<Scalar, Dim, Mode, Options> &t, const Track3 &f) {
    Transform<Scalar, Dim, Mode> res = t;
    res.matrix().applyOnTheRight(f.matrix());
    return res;
  }

  friend EIGEN_DEVICE_FUNC inline AffineTransformType operator*(
      const Translation<Scalar, 3> &other, const Track3 &f) {
    return AffineTransformType(other) * f;
  }

  friend EIGEN_DEVICE_FUNC inline AffineTransformType operator*(
      const UniformScaling<Scalar> &other, const Track3 &f) {
    return AffineTransformType(other) * f;
  }

  template <typename OtherDerived>
  friend EIGEN_DEVICE_FUNC inline AffineTransformType operator*(
      const EigenBase<OtherDerived> &linear, const Track3 &f) {
    return AffineTransformType(linear) * f;
  }

  template <typename Derived>
  friend EIGEN_DEVICE_FUNC inline AffineTransformType operator*(
      const RotationBase<Derived, 3> &r, const Track3 &f) {
    return AffineTransformType(r) * f;
  }

  template <typename NewScalarType>
  EIGEN_DEVICE_FUNC inline
      typename internal::cast_return_type<Track3, Track3<NewScalarType>>::type
      cast() const {
    return
        typename internal::cast_return_type<Track3,
                                            Track3<NewScalarType>>::type(*this);
  }

  EIGEN_DEVICE_FUNC inline operator AffineTransformType() const {
    return transform();
  }

  template <typename OtherScalarType>
  EIGEN_DEVICE_FUNC inline explicit Track3(
      const Track3<OtherScalarType> &other) {
    m_fwd   = other.fwd().template cast<Scalar>();
    m_up    = other.up().template cast<Scalar>();
    m_flipX = other.flipX();
  }

  EIGEN_DEVICE_FUNC bool isApprox(
      const Track3 &                   other,
      typename NumTraits<Scalar>::Real prec =
          NumTraits<Scalar>::dummy_precision()) const {
    return m_fwd.isApprox(other.m_fwd, prec) &&
           m_up.isApprox(other.m_up, prec) && m_flipX == other.m_flipX;
  }
};

typedef Track3<float>  Track3f;
typedef Track3<double> Track3d;
} // namespace Eigen
