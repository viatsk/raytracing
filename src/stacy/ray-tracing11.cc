#include "camera.h"
#include "sphere.h"
#include "world.h"

#include <iostream>

int main() {
  world world;
  // misc - we should see the reflection of this sphere in the mirrored surface.
  world.add(sphere(point3(0.4, 0.4, -1), 0.2, lambertian(colour(0.1, 0.2, 0.5))));
  // left sphere: bubble
  double refraction_glass_sphere = 1.5;
  world.add(sphere(point3(-1.1, 0.05, -1), 0.6, dielectric(refraction_glass_sphere)));
  world.add(sphere(point3(-1.1, 0.05, -1), 0.5, dielectric(/*air*/1.00 / refraction_glass_sphere)));
  // right sphere: metallic grey w/ 0.5 fuzz
  world.add(sphere(point3(1, 0, -1), 0.5, metallic(grey, 0.5)));
  // middle sphere: plain blue
  world.add(sphere(point3(0, 0, -1), 0.5, lambertian(blue)));
  // ground 
  world.add(sphere(point3(0,-100.5,-1), 100, metallic(light_green)));

  camera cam(/*camera_centre_=*/point3(0, 0, -3), /*vfov_=*/90, 16.0 / 9.0, 800 /*3840*/);
  cam.set_max_depth(3);
  cam.set_samples_per_pixel(1);
  cam.set_colour_mode(ColourMode::NumBounces);

  cam.render(world);

  return 0;
}
