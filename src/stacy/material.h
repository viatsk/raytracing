#ifndef MATERIAL_H
#define MATERIAL_H

#include "colour.h"
#include "hit_record.h"
#include "ray.h"
#include "vec3.h"

#include <tuple>

class lambertian {
 public:
  lambertian(const colour& albedo) : albedo_(albedo) {}

  // TODO: ray_in not used?
  // Implement cosine normal sampling, I think.
  std::tuple<colour, ray> scatter(const ray& r_in, const hit_record& rec) const {
      auto scatter_direction = rec.normal + random_unit_vector();
      // Catch degenerate scatter direction
      if (scatter_direction.near_zero())
          scatter_direction = rec.normal;

      return std::make_tuple(albedo_, ray(rec.p, scatter_direction));
  }

 private:
  colour albedo_;
};

#endif
