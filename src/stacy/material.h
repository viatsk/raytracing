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

class metallic {
 public:
  metallic(const colour& albedo) : albedo_(albedo) {}

  vec3 reflect(const vec3& v, const vec3& n) const {
    // This assumes the normal n is a unit vector.
    // If it wasn't, we need to divide the dot prod by length(n).
    return v - 2*dot(v,n)*n;
  }

  std::tuple<colour, ray> scatter(const ray& r_in, const hit_record& rec) const {
    vec3 reflection_direction = reflect(r_in.direction(), rec.normal);
    return std::make_tuple(albedo_, ray(rec.p, reflection_direction));
  }

 private:
  colour albedo_;
};


#endif
