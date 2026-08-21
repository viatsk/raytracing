#ifndef SPHERE_H
#define SPHERE_H

#include <optional>

#include "hittable.h"
#include "vec3.h"

// This should be used to set the hit record normal vector
// If the dot product of the ray and outward normal is positive,
// then we are inside the sphere. If it's negative we are outside
// the sphere. We always want to be outside the sphere so return
// -outward_normal if the ray is inside.
// This will become relevant for materials later.
vec3 normal_san(const ray& r, const vec3& outward_normal) {
    return (dot(r.direction(), outward_normal) < 0) ? outward_normal : -outward_normal;
}


class sphere : public hittable {
 public:
  sphere(const point3& centre, double radius, colour colour = blue, bool dont_shade_override = false) : centre_(centre), radius_(radius), colour_(colour), dont_shade_override_(dont_shade_override){}

  std::optional<hit_record> hit(const ray& r, double closest_so_far) const override {
    vec3 sphere_centre = centre_ - r.origin();
    double a = r.direction().length_squared();
    double h = dot(r.direction(), sphere_centre);
    double c = sphere_centre.length_squared() - (radius_ * radius_);
    double discriminant = h*h - a*c;

    if (discriminant < 0.0)
        return {};

    double root = (h - std::sqrt(discriminant)) / a;
    // If the root is negative, find the positive root
    if (root <= 0.001 || root >= closest_so_far) {
      root = (h + std::sqrt(discriminant)) / a;
      if (root <= 0.001 || root >= closest_so_far )
        return {};
    }

    hit_record hit;
    hit.t = root; // value of t for which ray is hit
    hit.p = r.at(root); // value of the traced ray at the root
    vec3 norm = (r.at(root) - centre_) / radius_;
    hit.normal = normal_san(r, norm);
    hit.hit_colour_REMOVEME = colour_; // TODO this is not a good way to shade the sphere
    hit.dont_shade = dont_shade_override_;
    return hit;
  }

 private:
  point3 centre_;
  double radius_;
  colour colour_;
  bool dont_shade_override_;
};

#endif

