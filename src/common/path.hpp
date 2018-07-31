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
  const Segment *m_curr, *m_prev;
  int            m_pos, m_end;
  Vec            m_vec;

  void validate() const;

public:
  constexpr const auto &curr() const {
    validate();
    return m_vec;
  }

  SegmentIter(const Segment &curr, const Segment *prev, int end) :
      m_curr(&curr),
      m_prev(prev),
      m_pos(0),
      m_end(end) {}

  bool next();
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

  Segment(SegmentType _type, const Vec &_to) : type(_type), to(_to) {}
  Segment(const Segment &);

  Vec sample(const Segment *prev, Scalar t) const;

  // TODO: fix res
  SegmentIter iter(const Segment *prev, int res) const;
};

class Figure;

class FigureIter {
  std::vector<Segment>::const_iterator m_it, m_end;
  SegmentIter                          m_curr;
  int                                  m_res;

public:
  constexpr const auto &curr() const { return m_curr.curr(); }

  FigureIter(const Figure &fig, int res);

  bool next();
};

class Figure {
  std::vector<Segment> m_segs;
  bool                 m_closed = false;

  Segment &put(SegmentType, const Vec &);

public:
  constexpr auto &closed() const { return m_closed; }

  explicit Figure(const Vec &);

  void line(const Vec &);

  void arc(const Vec &v, Scalar r);

  void bezier(const Vec &a, const Vec &b, const Vec &v);

  void close() { m_closed = true; }

  FigureIter iter(int res) const { return FigureIter(*this, res); }

  friend class FigureIter;
};

inline FigureIter::FigureIter(const Figure &fig, int res) :
    m_it(fig.m_segs.begin()),
    m_end(fig.m_segs.end()),
    m_curr(m_it->iter(nullptr, res)),
    m_res(res) {}

class Path {
  std::vector<Figure> m_figs;

public:
  constexpr const auto &figs() const { return m_figs; }
  Figure &              curr() { return m_figs.back(); }
  const Figure &        curr() const { return m_figs.back(); }

  Path &open(const Vec &);

  Path &close() {
    curr().close();
    return *this;
  }

  Path &line(const Vec &v) {
    curr().line(v);
    return *this;
  }

  Path &arc(const Vec &v, Scalar r) {
    curr().arc(v, r);
    return *this;
  }

  Path &bezier(const Vec &a, const Vec &b, const Vec &v) {
    curr().bezier(a, b, v);
    return *this;
  }

  auto begin() const { return m_figs.begin(); }
  auto end() const { return m_figs.end(); }
};

Scalar lineVsLine1(const Vec &u, const Vec &v, const Vec &p, const Vec &q);

std::pair<Scalar, Scalar> lineVsLine(const Vec &u,
                                     const Vec &v,
                                     const Vec &p,
                                     const Vec &q);

Vec miter(const Vec &a, const Vec &b, Scalar r);

Vec miter(const Vec &a, const Vec &b, const Vec &c, Scalar r1, Scalar r2);

std::pair<std::vector<Eigen::Vector3f>,
          std::vector<std::array<std::uint16_t, 3>>>
stroke(const Path &path, int res, float w);
} // namespace pth
