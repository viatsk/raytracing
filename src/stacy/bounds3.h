#ifndef BOUNDS3_H
#define BOUNDS3_H

#include "hit_record.h"
#include "vec3.h"

#include <optional>

class bounds3 {
 public:
  // Constructors
  bounds3() {} // TODO: Make this into min_ = min float, max_ = max float?
  bounds3(const point3 &p): min_(p), max_(p) {}
  bounds3(const point3 &p1, const point3& p2):
        min_(std::min(p1.x(), p2.x()),
             std::min(p1.y(), p2.y()),
             std::min(p1.z(), p2.z())),
        max_(std::max(p1.x(), p2.x()),
             std::max(p1.y(), p2.y()),
             std::max(p1.z(), p2.z())) {}
  // Equals operator
  bool operator==(const bounds3& b) const {
    return b.min_ == min_ && b.max_ == max_;
  }
  bool operator!=(const bounds3& b) const {
    return !(*this == b);
  }

  // Helper functions
  // Returns the value at the 0..8th corner
  point3 corner(int corner) const {
    switch (corner) {
        case 0: return min_;
        case 1: return point3(max_.x(), min_.y(), min_.z());
        case 2: return point3(min_.x(), max_.y(), min_.z());
        case 3: return point3(max_.x(), max_.y(), min_.z());
        case 4: return point3(min_.x(), min_.y(), max_.z());
        case 5: return point3(max_.x(), min_.y(), max_.z());
        case 6: return point3(min_.x(), max_.y(), max_.z());
        case 7: return max_;
        default: return min_;
    }
  }

  point3 min_;
  point3 max_;
};

// bounds3 Utility Functions
// TODO: Use extants instead.
inline std::ostream& operator<<(std::ostream& out, const bounds3& b) {
    out << "bounds3{";
    for (int i = 0; i < 8; i++) {
       out << b.corner(i) << " ";
    }
    return out << "}";
}

inline point3 min_bound(const bounds3& b1, const bounds3& b2) {
return point3(std::min(b1.min_.x(), b2.min_.x()),
                std::min(b1.min_.y(), b2.min_.y()),
                std::min(b1.min_.z(), b2.min_.z()));
}

inline point3 max_bound(const bounds3& b1, const bounds3& b2) {
return point3(std::max(b1.max_.x(), b2.max_.x()),
                std::max(b1.max_.y(), b2.max_.y()),
                std::max(b1.max_.z(), b2.max_.z()));
}

inline bounds3 box_union(const bounds3 &b1, const bounds3 &b2) {
  return bounds3(min_bound(b1, b2), max_bound(b1, b2));
}

#endif

