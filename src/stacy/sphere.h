#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"


class sphere : public hittable {
 public:
  sphere(const point3& centre, double radius) : centre_(centre), radius_(radius) {}

  bool hit(const ray& r, hit_record& hit_record) const override {
    vec3 sphere_centre = centre_ - r.origin();
    double a = r.direction().length_squared();
    double h = dot(r.direction(), sphere_centre);
    double c = sphere_centre.length_squared() - (radius_ * radius_);
    double discriminant = h*h - a*c;

    if (discriminant < 0.0)
        return false;

    double root = (h - std::sqrt(discriminant)) / a;
    // If the root is negative, find the positive root
    if (root <= 0){
      root = (h + std::sqrt(discriminant)) / a;
      if (root <= 0)
        return false;
    }

    hit_record.t = root; // value of t for which ray is hit
    hit_record.p = r.at(root); // value of the traced ray at the root
    hit_record.normal = (r.at(root) - centre_) / radius_;
    return true;
  }

 private:
  point3 centre_;
  double radius_;

};

#endif

