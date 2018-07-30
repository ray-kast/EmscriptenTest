#include "path.hpp"

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

bool FigureIter::next() {
  while (!m_curr.next()) {
    auto prev = &*m_it;

    ++m_it;

    if (m_it == m_end) return false;

    m_curr = m_it->iter(prev, m_res);
  }

  return true;
}

Segment &Figure::put(SegmentType type, Vec v) {
  return m_segs.emplace_back(type, v);
}

Figure::Figure(Vec v) { put(Begin, v); }

void Figure::line(Vec v) { put(Line, v); }

void Figure::arc(Vec v, Scalar r) {
  auto &seg = put(Arc, v);

  seg.arc.radius = std::abs(r);
  seg.arc.pos    = r >= 0;
}

void Figure::bezier(Vec a, Vec b, Vec v) {
  auto &seg = put(Bezier, v);

  seg.bez.a3 = a * Scalar(3);
  seg.bez.b3 = b * Scalar(3);
}

Path &Path::open(Vec v) {
  m_figs.emplace_back(v);
  return *this;
}
} // namespace pth
