#ifndef CAMERA_H
#define CAMERA_H

#include "colour.h"
#include "world.h"

#include <optional>

const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

class camera {
 public:
  camera(const point3& camera_centre, double vfov, double aspect_ratio = 16.0 / 9.0, int img_width = 1380 ) : camera_centre_(camera_centre), vfov_(vfov), aspect_ratio_(aspect_ratio), img_width_(img_width) {
    img_height_ = int(img_width / aspect_ratio);

    auto focal_length = (camera_centre_ - lookat).length();
    auto theta = degrees_to_radians(vfov_);
    auto h = std::tan(theta/2);
    auto viewport_height = 2 * h * focal_length;
    auto viewport_width = viewport_height * (double(img_width)/img_height_);

    w = unit_vector(camera_centre_ - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // These vectors represent viewport traversal deltas in the horizontal and vertical
    vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
    vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical deltas, which are also vectors
    pixel_delta_u = viewport_u / img_width;
    pixel_delta_v = viewport_v / img_height_;

    vec3 Q = camera_centre_ - (focal_length * w) - viewport_u/2 - viewport_v/2;
    pixel00_loc = Q + (0.5 * (pixel_delta_u + pixel_delta_v));
  }

  void render(const world& world) {
    // Write PPM file
    std::cout << "P3\n" << img_width_ << ' ' << img_height_ << "\n255\n";

    for (int h = 0; h < img_height_; h++) {
      std::clog << "\r Scanlines remaining: " << (img_height_ - h) <<  " " << std::flush;
        for (int w = 0; w < img_width_; w++) {
          vec3 pixel_centre = pixel00_loc + (w * pixel_delta_u) + (h * pixel_delta_v);
          vec3 ray_direction = pixel_centre - camera_centre_;
          ray r(camera_centre_, ray_direction); // So all rays come from the camera centre.
          vec3 pixel_colour = ray_colour(r, world);
          write_colour(std::cout, pixel_colour);
      }
    }
    std::clog << "\r Done! \n";
  }

  point3 lookat   = point3(0,0,-1);  // Point camera is looking at
  vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction TODO understand better

 private:
  void initialize() {
  }

  vec3 stacy_lerp(vec3 start_value, vec3 end_value, float a) {
    return (1.0-a)*start_value + a*end_value;
  }

  colour ray_colour(const ray& r, const world& world) {
    std::optional<hit_record> record = world.hit(r, std::numeric_limits<double>::infinity());
    if (record.has_value()) {
      // TODO: This is a bad way to not shade one of the spheres.
      if (record.value().sphere_colour == colour(0.565, 0.933, 0.565))
        return colour(0.565, 0.933, 0.565);
      return 0.5 * (record.value().normal + white) + record.value().sphere_colour;
    }

    // Background gradient
    vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);

    return stacy_lerp(blue, white, a);
  }

  // Image dimensions
  double aspect_ratio_;
  int img_width_;
  int img_height_;

  point3 camera_centre_;
  double vfov_; // Vertical field of view
  vec3 pixel00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;
  vec3 u, v, w;              // Camera frame basis vectors
};

#endif