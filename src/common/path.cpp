#include "path.hpp"

#include <array>

#include "diag.hpp"

namespace pth {
void SegmentIter::validate() const {
#if !defined(_NDEBUG)
  if (!m_pos)
    die("attempt to dereference before-the-beginning segment iterator");

  if (!m_curr) die("attempt to dereference singular segment iterator");

  if (m_pos > m_end)
    die("attempt to dereference past-the-end segment iterator");
#endif
}

bool SegmentIter::next() {
#if !defined(_NDEBUG)
  if (m_pos > m_end) die("attempt to increment past-the-end segment iterator");
#endif

  ++m_pos;

  if (m_pos > m_end) return false;

  m_vec = m_curr->sample(m_prev, Scalar(m_pos) / Scalar(m_end));

  return true;
}

Segment::Segment(const Segment &other) : type(other.type), to(other.to) {
  switch (type) {
  case Begin:
  case Line: break;
  case Arc: arc = decltype(arc)(other.arc); break;
  case Bezier: bez = decltype(bez)(other.bez); break;
  }
}

Vec Segment::sample(const Segment *prev, Scalar t) const {
  switch (type) {
  case Begin: return to;
  case Line: return lerp(prev->to, to, t);
  case Arc: {
    Vec offs = to - prev->to;

    Scalar opp   = offs.norm() / Scalar(2),
           alpha = std::asin(opp / arc.radius) * Scalar(arc.pos ? 1 : -1);

    Vec center = lerp(prev->to, to, 0.5);

    offs.reverseInPlace();
    offs = offs.cwiseProduct(Vec(1, -1));
    offs.normalize();

    center += -offs * opp / std::tan(alpha);

    Vec    ac     = prev->to - center;
    Scalar angleA = std::atan2(ac.y(), ac.x()),
           angle  = lerp(angleA, angleA + alpha * 2, t);

    return center + arc.radius * Vec(std::cos(angle), std::sin(angle));
  }
  case Bezier: {
    Vec prev3 = prev->to * Scalar(3);

    // clang-format off
    return prev->to
      + t * (bez.a3 - prev3
        + t * (prev3 - Scalar(2) * bez.a3 + bez.b3
          + t * (to - prev->to + bez.a3 - bez.b3)));
    // clang-format on
  }
  }
}

SegmentIter Segment::iter(const Segment *prev, int res) const {
  switch (type) {
  case Begin:
  case Line: res = 1; break;
  case Arc:
  case Bezier: break;
  }
  return SegmentIter(*this, prev, res);
}

bool FigureIter::next() {
  while (!m_curr.next()) {
    auto prev = &*m_it;

    ++m_it;

    if (m_it == m_end) return false;

    m_curr = m_it->iter(prev, m_res);
  }

  return true;
}

Segment &Figure::put(SegmentType type, const Vec &v) {
  return m_segs.emplace_back(type, v);
}

Figure::Figure(const Vec &v) { put(Begin, v); }

void Figure::line(const Vec &v) { put(Line, v); }

void Figure::arc(const Vec &v, Scalar r) {
  auto &seg = put(Arc, v);

  seg.arc.radius = std::abs(r);
  seg.arc.pos    = r >= 0;
}

void Figure::bezier(const Vec &a, const Vec &b, const Vec &v) {
  auto &seg = put(Bezier, v);

  seg.bez.a3 = a * Scalar(3);
  seg.bez.b3 = b * Scalar(3);
}

Path &Path::open(const Vec &v) {
  m_figs.emplace_back(v);
  return *this;
}

Scalar lineVsLine1(const Vec &u, const Vec &v, const Vec &p, const Vec &q) {
  Vec d = p - u;

  return (q.y() * d.x() - q.x() * d.y()) / (q.y() * v.x() - q.x() * v.y());
}

std::pair<Scalar, Scalar> lineVsLine(const Vec &u,
                                     const Vec &v,
                                     const Vec &p,
                                     const Vec &q) {
  Vec d = p - u;

  Scalar s = (q.y() * d.x() - q.x() * d.y()) / (q.y() * v.x() - q.x() * v.y()),
         t = std::abs(q.x()) > std::abs(q.y()) ? (s * v.x() - d.x()) / q.x()
                                               : (s * v.y() - d.y()) / q.y();


  return std::pair(s, t);
}

Vec miter(const Vec &a, const Vec &b, Scalar r) {
  Vec ortho = a - b;
  ortho.reverseInPlace();
  ortho = ortho.cwiseProduct(Vec(-1, 1)).eval();
  ortho.normalize();
  return r * ortho;
}

Vec miter(const Vec &a, const Vec &b, const Vec &c, Scalar r, Scalar s) {
  Vec aOffs = a + miter(a, b, r), ab = b - a;

  return aOffs + ab * lineVsLine1(aOffs, ab, c - miter(c, b, s), b - c) - b;
}

std::pair<std::vector<Eigen::Vector3f>,
          std::vector<std::array<std::uint16_t, 3>>>
stroke(const Path &path, int res, float w) {
  std::vector<Eigen::Vector3f>              pos;
  std::vector<std::array<std::uint16_t, 3>> idx;

  for (auto fig : path) {
    auto it = fig.iter(res);

    int i = 0, i0 = pos.size();
    Vec a, b, v0, v1;

    while (it.next()) {
      auto vec = it.curr();

      switch (i) {
      case 0: v0 = vec; goto next;
      case 1:
        v1 = vec;
        a  = miter(b, vec, w);
        break;
      default: a = miter(a, b, vec, w, w); break;
      }

      {
        Vec v = b - a;
        pos.emplace_back(Eigen::Vector3f(v.x(), v.y(), 0.0f));
        v = b + a;
        pos.emplace_back(Eigen::Vector3f(v.x(), v.y(), 0.0f));

        std::uint16_t ii = static_cast<std::uint16_t>(i0 + i + i);

        idx.push_back(
            std::array<std::uint16_t, 3>{{static_cast<std::uint16_t>(ii - 2),
                                          static_cast<std::uint16_t>(ii - 1),
                                          static_cast<std::uint16_t>(ii + 1)}});
        idx.push_back(
            std::array<std::uint16_t, 3>{{static_cast<std::uint16_t>(ii + 1),
                                          ii,
                                          static_cast<std::uint16_t>(ii - 2)}});
      }

    next:
      ++i;
      a = b;
      b = vec;
    }

    if (fig.closed()) {
      Vec v;

      if (i > 0) {
        a = miter(a, b, v0, w, w);

        v = b - a;
        pos.emplace_back(Eigen::Vector3f(v.x(), v.y(), 0.0f));
        v = b + a;
        pos.emplace_back(Eigen::Vector3f(v.x(), v.y(), 0.0f));

        std::uint16_t ii = static_cast<std::uint16_t>(i0 + i + i);

        idx.push_back(
            std::array<std::uint16_t, 3>{{static_cast<std::uint16_t>(ii - 2),
                                          static_cast<std::uint16_t>(ii - 1),
                                          static_cast<std::uint16_t>(i0 + 1)}});
        idx.push_back(
            std::array<std::uint16_t, 3>{{static_cast<std::uint16_t>(i0 + 1),
                                          static_cast<std::uint16_t>(i0),
                                          static_cast<std::uint16_t>(ii - 2)}});
      }

      if (i > 1) {
        a = miter(b, v0, v1, w, w);

        v              = v0 - a;
        pos.at(i0)     = Eigen::Vector3f(v.x(), v.y(), 0.0f);
        v              = v0 + a;
        pos.at(i0 + 1) = Eigen::Vector3f(v.x(), v.y(), 0.0f);
      }
    }
    else {
      a = miter(a, b, w);

      Vec v = b - a;
      pos.emplace_back(Eigen::Vector3f(v.x(), v.y(), 0.0f));
      v = b + a;
      pos.emplace_back(Eigen::Vector3f(v.x(), v.y(), 0.0f));
    }
  }

  return std::make_pair(pos, idx);
}
} // namespace pth
