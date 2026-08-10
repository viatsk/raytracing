#ifndef CAMERA_H
#define CAMERA_H

#include "colour.h"
#include "world.h"

#include <optional>

class camera {
 public:
  void render(const world& world) {
    initialize();

    // Render
    std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

    for (int h = 0; h < img_height; h++) {
      std::clog << "\r Scanlines remaining: " << (img_height - h) <<  " " << std::flush;
        for (int w = 0; w < img_width; w++) {
          vec3 pixel_centre = pixel00_loc + (w * pixel_delta_u) + (h * pixel_delta_v);
          vec3 ray_direction = pixel_centre - camera_centre;
          ray r(camera_centre, ray_direction); // So all rays come from the camera centre.
          vec3 pixel_colour = ray_colour(r, world);
          write_colour(std::cout, pixel_colour);
      }
    }
    std::clog << "\r Done! \n";
  }

  // Public member variables
  // TODO: Make these initialization params
  double aspect_ratio = 16.0 / 9.0;
  int img_width = 1280;


 private:
  void initialize() {
    img_height = int(img_width / aspect_ratio);

    // Viewport
    auto viewport_height = 2.0; // Why suggested 2?
    auto viewport_width = viewport_height * (double(img_width)/img_height);
    // These vectors represent viewport traversal deltas in the horizontal and vertical
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical deltas, which are also vectors
    pixel_delta_u = viewport_u / img_width;
    pixel_delta_v = viewport_v / img_height;

    vec3 Q = camera_centre - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
    pixel00_loc = Q + (0.5 * (pixel_delta_u + pixel_delta_v));
  }

  vec3 stacy_lerp(vec3 start_value, vec3 end_value, float a) {
    return (1.0-a)*start_value + a*end_value;
  }

  colour ray_colour(const ray& r, const world& world) {
    std::optional<hit_record> record = world.hit(r, std::numeric_limits<double>::infinity());
    if (record.has_value()) {
        return 0.5 * (record.value().normal + white);
    }

    // Background gradient
    vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);

    return stacy_lerp(blue, white, a);
  }

  // Image dimensions
  int img_height;

  // Primitive Camera
  double focal_length = 1.0; // TODO this should be an initialization param
  point3 camera_centre = point3(0, 0, 0); // TODO this should be an initialization param 
  vec3 pixel00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;

};

#endif