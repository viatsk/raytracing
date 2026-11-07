#ifndef BOUNDS3_H
#define BOUNDS3_H

#include "hit_record.h"
#include "vec3.h"

#include <optional>

static constexpr double kMinDouble = std::numeric_limits<double>::lowest();
static constexpr double kMaxDouble = std::numeric_limits<double>::max();

class bounds3 {
 public:
  // Constructors
  bounds3() : min_(kMaxDouble, kMaxDouble, kMaxDouble), max_(kMinDouble, kMinDouble, kMinDouble){} // TODO: Make this into min_ = min float, max_ = max float?
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
  double surface_area() const {
    double length = max_.x() - min_.x();
    double width = max_.y() - min_.y();
    double height = max_.z() - min_.z();
    return 2 * (length*width + width*height + length*height);
  }

  point3 centroid() const {
    return (0.5 * min_) + (0.5 * max_);
  }

  vec3 diagonal() const {
    return max_ - min_;
  }

  int max_extent() const {
    vec3 diag = diagonal();
    return (diag.x() > diag.y()) ? 
                        (diag.x() > diag.z() ? 0 : 2)
                      : (diag.y() > diag.z() ? 1 : 2);
  }

  // offset() returns the continuous position of a
  // point relative to the corners of the box,
  // where a point at the minimum corner has offset of (0, 0, 0)
  // and a point at the maximum corner has offset of (1, 1, 1). 
  vec3 offset(const point3 &p) const {
    vec3 offset = p - min_;
    for (int axis : {0, 1, 2}) {
      if (max_[axis] > min_[axis]) {
        offset[axis] /= (max_[axis] - min_[axis]);
      }
    }
    return offset;
  }

  // Filtering on the hit test function that allows the AABB be drawn.
  // TODO: Make this into a ray–edge distance test instead of filtering
  // intersections.
  std::optional<hit_record> hit_with_colour_info(const ray& r) const {
    auto intersection_t = intersects(r);
    if (!intersection_t.has_value())
      return {};
    
    // Use the entry point, if the ray starts inside the AABB
    // then we'd need to check the exit distance (smallest_tmax)
    // TODO^?
    const point3 p = r.at(intersection_t.value());
    int boundary_count = 0;

    double delta = 0.02;
    for (int axis : {0, 1, 2}) {
        if (std::abs(p[axis] - min_[axis]) <= delta ||
            std::abs(p[axis] - max_[axis]) <= delta) {
            boundary_count++;
        }
    }
    if (boundary_count < 2)
        return {};
    
    hit_record hit;
    hit.p = p;
    // normal? 
    hit.t = intersection_t.value();
    hit.attenuation = red;
    hit.scattered = ray(p, r.direction());
    hit.num_hit_tests = 1;
    return hit;
  }

  // Normal hit testing function! 
  bool hit(const ray& r) const {
    return intersects(r).has_value();
  }

  std::optional<double> intersects(const ray& r) const {
    const point3& ray_orig = r.origin();
    const vec3&   ray_dir  = r.direction();
    double smallest_tmax = INFINITY;
    double largest_tmin = -INFINITY;

    // Slab method on each cardinal direction
    for (int axis : {0, 1, 2}) {
      const double inverse_direction = 1.0 / ray_dir[axis];
      auto tmin = (min_[axis] - ray_orig[axis]) * inverse_direction;
      auto tmax = (max_[axis] - ray_orig[axis]) * inverse_direction;

      if (tmin > tmax) {
        std::swap(tmin, tmax);
      }
      // Set the ray cube entrance, exit points
      largest_tmin = std::max(largest_tmin, tmin);
      smallest_tmax = std::min(smallest_tmax, tmax);
    }

    // If the extrema are inversed, then this isnt a valid hit
    // TODO: This may need to be <= max(largest_tmin, 0) if
    // the intersection point is behind the ray?
    if (smallest_tmax <= largest_tmin) {
      return {};
    }
    
    return largest_tmin;
  }

  point3 min_;
  point3 max_;
};

// bounds3 Utility Functions
inline std::ostream& operator<<(std::ostream& out, const bounds3& b) {
    out << "bounds3{" << b.min_ << " to " << b.max_;
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

