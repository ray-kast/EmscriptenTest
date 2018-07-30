#pragma once

#include <vector>

#include <Eigen/Eigen>

namespace pth {
using Vec    = Eigen::Vector2f;
using Scalar = Vec::Scalar;

template <typename T>
inline constexpr auto lerp(T a, T b, Scalar f) {
  return a + f * (b - a);
}

inline constexpr Scalar deg(Scalar d) { return d * Scalar(M_PI) / Scalar(180); }
inline constexpr Scalar grad(Scalar g) { return g * Scalar(2) * Scalar(M_PI); }

namespace lit {
  inline constexpr Scalar operator""_sc(unsigned long long scalar) {
    return Scalar(scalar);
  }

  inline constexpr Scalar operator""_sc(long double scalar) {
    return Scalar(scalar);
  }

  inline constexpr Scalar operator""_deg(unsigned long long d) {
    return deg(Scalar(d));
  }

  inline constexpr Scalar operator""_deg(long double d) {
    return deg(Scalar(d));
  }

  inline constexpr Scalar operator""_grad(unsigned long long g) {
    return grad(Scalar(g));
  }

  inline constexpr Scalar operator""_grad(long double g) {
    return grad(Scalar(g));
  }
} // namespace lit

enum SegmentType {
  Begin,
  Line,
  Arc,
  Bezier,
};

struct Segment;

class SegmentIter {
  const Segment *m_curr = nullptr, *m_prev = nullptr;
  int            m_pos = 0, m_end = 0;
  Vec            m_vec;

  void resample();

  void validate() const;

public:
  SegmentIter() = default;

  SegmentIter(const Segment &curr, const Segment *prev, int end, bool atEnd);

  const Vec *operator->() const {
    validate();
    return &m_vec;
  }
  const Vec &operator*() const {
    validate();
    return m_vec;
  }

  SegmentIter &operator++();

  bool operator==(const SegmentIter &) const;
  bool operator!=(const SegmentIter &rhs) const { return !operator==(rhs); }
};

struct Segment {
  SegmentType type;
  Vec         to;

  union {
    struct {
      Scalar radius;
      bool   pos;
    } arc;

    struct {
      Vec a3, b3;
    } bez;
  };

  Segment(SegmentType _type, Vec _to) : type(_type), to(_to) {}
  Segment(const Segment &);

  Vec sample(const Segment *prev, Scalar t) const;

  SegmentIter begin(const Segment *prev, int res) const {
    return SegmentIter(*this, prev, res, false);
  }

  SegmentIter end(const Segment *prev, int res) const {
    return SegmentIter(*this, prev, res, true);
  }
};

class Figure {
  std::vector<Segment> m_segs;
  bool                 m_closed = false;

public:
  constexpr auto &closed() const { return m_closed; }

  Figure(Vec);

  void line(Vec);

  void arc(Vec v, Scalar r);

  void bezier(Vec a, Vec b, Vec v);

  void close();
};

class Path {
  std::vector<Figure> m_figs;

public:
  Figure &      curr() { return m_figs.back(); }
  const Figure &curr() const { return m_figs.back(); }

  Path &open(Vec);

  Path &close() {
    curr().close();
    return *this;
  }

  Path &line(Vec v) {
    curr().line(v);
    return *this;
  }

  Path &arc(Vec v, Scalar r) {
    curr().arc(v, r);
    return *this;
  }

  Path &bezier(Vec a, Vec b, Vec v) {
    curr().bezier(a, b, v);
    return *this;
  }
};
} // namespace pth
