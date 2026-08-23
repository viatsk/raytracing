#include "colour.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& centre, double radius, const ray& r){
  vec3 sphere_centre = centre - r.origin();
  double a = r.direction().length_squared();
  double h = dot(r.direction(), sphere_centre);
  double c = sphere_centre.length_squared() - (radius * radius);
  double discriminant = h*h - a*c;

  if (discriminant < 0.0)
    return -1.0; // If imaginary sol'n, do nothing
  else 
    return (h- std::sqrt(discriminant)) /  a;
}

colour ray_colour(const ray& r) {
  vec3 unit_direction = unit_vector(r.direction());

  // // Objects in the scene
  vec3 red_sphere_centre = point3(0, 0, 1.0);
  double t = hit_sphere(red_sphere_centre, 0.1, r);
  if (t > 0.0){
    vec3 normalized_colour = unit_vector(r.at(t) - red_sphere_centre);
    return 0.5 * (normalized_colour + white) + red;
  }

  vec3 green_sphere_centre = point3(-0.2, 0, 1.0);
  t = hit_sphere(green_sphere_centre, 0.1, r);
  if (t > 0.0){
    vec3 normalized_colour = unit_vector(r.at(t) - green_sphere_centre);
    return 0.5 * (normalized_colour + white) + green;
  }

  vec3 blue_sphere_centre = point3(0.2, 0, 1.0);
  t = hit_sphere(blue_sphere_centre, 0.1, r);
  if (t > 0.0){
    vec3 normalized_colour = unit_vector(r.at(t) - blue_sphere_centre);
    return 0.5 * (normalized_colour + white) + blue;
  }

  // When a = 1, blue. When a = 0, white. 
  double a = 0.5 * (unit_direction.y() + 1.0);
  vec3 white_part = (1.0-a)*colour(1.0, 1.0, 1.0);
  vec3 blue_part = a*colour(0.0, 0.0, 1.0);

  return white_part + blue_part;
}


int main() {
  // Image
  auto aspect_ratio = 16.0 / 9.0;
  int img_width = 1280;
  int img_height = int(img_width / aspect_ratio);

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

        // vec3 pixel_centre = pixel00_loc + (w * pixel_delta_u) + (h * pixel_delta_v);
        vec3 ray_direction = vec3(u, v, focal_length);
        ray r(camera_centre, ray_direction); // So all rays come from the camera centre.
        vec3 pixel_colour = ray_colour(r);
        write_colour(std::cout, pixel_colour);
    }
  }

  std::clog << "\r Done! \n";
  return 0;
}

