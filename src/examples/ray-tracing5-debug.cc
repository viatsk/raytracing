#include "colour.h"
#include "sphere.h"
#include "world.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

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

int main() {
  // Image
  auto aspect_ratio = 16.0 / 9.0;
  int img_width = 1280;
  int img_height = int(img_width / aspect_ratio);

  // World
  world world;
  
  world.add(sphere(point3(0, 0, 1.0), 0.1));
  world.add(sphere(point3(-0.2, 0, 1.0), 0.1));
  world.add(sphere(point3(0.2, 0, 1.0), 0.1));
  world.add(sphere(point3(0, 100, 15.0), 100));

  // Primitive Camera
  double focal_length = 1.0;
  point3 camera_centre = point3(0, 0, 0);

  // Render
  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

  for (int h = 0; h < img_height; h++) {
    std::clog << "\r Scanlines remaining: " << (img_height - h) <<  " " << std::flush;
      for (int w = 0; w < img_width; w++) {
        double u = (w - (0.5 * img_width)) / img_height;
        double v = (h - (0.5 * img_height)) / img_height;
        vec3 ray_direction = vec3(u, v, focal_length);
        ray r(camera_centre, ray_direction); // So all rays come from the camera centre.
        vec3 pixel_colour = ray_colour(r, world);
        write_colour(std::cout, pixel_colour);
    }
  }

  std::clog << "\r Done! \n";
  return 0;
}


