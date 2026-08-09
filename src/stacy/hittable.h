#ifndef HITTABLE_H
#define HITTABLE_H

#include <optional>

#include "ray.h"


struct hit_record {
  point3 p;
  vec3 normal;
  double t;
};

class hittable {
 public:
  virtual ~hittable() = default; // needs to be virtual for correct destruction
  virtual std::optional<hit_record> hit(const ray& r, double closest_so_far) const = 0;
};

#endif

