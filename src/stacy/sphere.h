#ifndef SPHERE_H
#define SPHERE_H

#include <optional>

#include "hittable.h"
#include "vec3.h"


class sphere : public hittable {
 public:
  sphere(const point3& centre, double radius, colour colour) : centre_(centre), radius_(radius), colour_(colour){}

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
    if (root <= 0 || root >= closest_so_far) {
      root = (h + std::sqrt(discriminant)) / a;
      if (root <= 0 || root >= closest_so_far )
        return {};
    }

    hit_record hit;
    hit.t = root; // value of t for which ray is hit
    hit.p = r.at(root); // value of the traced ray at the root
    hit.normal = (r.at(root) - centre_) / radius_;
    hit.sphere_colour = colour_; // TODO this is not a good way to shade the sphere
    return hit;
  }

 private:
  point3 centre_;
  double radius_;
  colour colour_;
};

#endif

