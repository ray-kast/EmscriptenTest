#include "path.hpp"

#include "diag.hpp"

namespace pth {
void SegmentIter::resample() {
  m_vec = m_curr->sample(m_prev, Scalar(m_pos) / Scalar(m_end));
}

void SegmentIter::validate() const {
#if !defined(_NDEBUG)
  if (!m_curr) die("attempt to dereference singular segment iterator");

  if (m_pos > m_end)
    die("attempt to dereference past-the-end segment iterator");
#endif
}

SegmentIter::SegmentIter(const Segment &curr,
                         const Segment *prev,
                         int            end,
                         bool           atEnd) :
    m_curr(&curr),
    m_prev(prev),
    m_pos(atEnd ? end + 1 : 1),
    m_end(end) {
  resample();
}

SegmentIter &SegmentIter::operator++() {
#if !defined(_NDEBUG)
  if (m_pos > m_end) die("attempt to increment past-the-end segment iterator");
#endif

  ++m_pos;
  resample();

  return *this;
}

bool SegmentIter::operator==(const SegmentIter &rhs) const {
  if (!m_curr && !rhs.m_curr) return true; // Both are singular

  if (!(m_curr == rhs.m_curr && m_prev == rhs.m_prev && m_end == rhs.m_end))
#if defined(NDEBUG)
    return false;
#else
    die("attempt to compare mismatched segment iterators");
#endif

  if (m_pos > m_end && rhs.m_pos > rhs.m_end)
    return true; // Both are past-the-end

  return m_pos == rhs.m_pos;
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
} // namespace pth
