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
  metallic(const colour& albedo, double fuzz) : albedo_(albedo), fuzz_(fuzz) {} 

  // TODO make into a helper fn
  vec3 reflect(const vec3& v, const vec3& n) const {
    // This assumes the normal n is a unit vector.
    // If it wasn't, we need to divide the dot prod by length(n).
    return v - 2*dot(v,n)*n;
  }

  std::tuple<colour, ray> scatter(const ray& r_in, const hit_record& rec) const {
    vec3 reflection_direction = reflect(r_in.direction(), rec.normal);
    vec3 fuzzed_reflextion_direction = unit_vector(reflection_direction) + (fuzz_ * random_unit_vector());
    return std::make_tuple(albedo_, ray(rec.p, fuzzed_reflextion_direction));
  }

 private:
  colour albedo_;
  double fuzz_ = 0.0; // default no fuzz
};

class dielectric {
 public:
  dielectric(double refraction_idx) : refraction_idx_(refraction_idx) {}

  // TODO make into a helper fn
  vec3 reflect(const vec3& v, const vec3& n) const {
    // This assumes the normal n is a unit vector.
    // If it wasn't, we need to divide the dot prod by length(n).
    return v - 2*dot(v,n)*n;
  }

  // assumes both incident and n are unit vectors!
  vec3 refract(const vec3& indicent, const vec3& n, double refractive_idx_ratio) const {
    auto cos_theta = std::fmin(dot(-indicent, n), 1.0);
    vec3 ray_perp = refractive_idx_ratio * (indicent + (cos_theta * n));
    vec3 ray_paralell = - std::sqrt(std::fabs(1.0 - ray_perp.length_squared())) * n;
    return ray_perp + ray_paralell;

  }
  std::tuple<colour, ray> scatter(const ray& r_in, const hit_record& rec) const {
    // 1.0 is the refraction index of air for our purposes (?)
    double refraction_ratio = (rec.inner_face) ? refraction_idx_ : (1.0 / refraction_idx_);
    vec3 unit_direction_r_in = unit_vector(r_in.direction());

    // Solve for sin_theta, cos_theta.
    double cos_theta = std::fmin(dot(-unit_direction_r_in, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    vec3 scatter_ray;
    if (((refraction_ratio * sin_theta) > 1.0)  ||
         (reflectance(cos_theta, refraction_ratio) > random_double_zero_to_one())) {
      // Cannot refract this ray - it gets reflected
      scatter_ray = reflect(unit_direction_r_in, rec.normal);
    } else {
      scatter_ray = refract(unit_direction_r_in, rec.normal, refraction_ratio);
    }

    return std::make_tuple(/* attenuation= */white, ray(rec.p, scatter_ray));
  }

 private:
  // The refraction index of the relevant materia.
  double refraction_idx_;

  static double reflectance (double cosine, double refraction_index) {
    // Use Schlick's approximation for reflectance.
    double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0 * r0;
    return (r0 + (1 - r0)*std::pow((1 - cosine), 5));
  }
};


#endif
