#include "camera.h"
#include "sphere.h"
#include "world.h"

#include <iostream>

int main() {

  world world;
  world.add(sphere(point3(0, 0, -1), 0.5, lambertian(colour(0.1, 0.2, 0.5))));
  // left sphere: bubble
  double refraction_glass_sphere = 1.5;
  world.add(sphere(point3(-1, 0, -1), 0.5, dielectric(refraction_glass_sphere)));
  world.add(sphere(point3(-1, 0, -1), 0.4, dielectric(/*air*/1.00 / refraction_glass_sphere)));
  // right sphere: metallic grey w/ 0.5 fuzz
  world.add(sphere(point3(1, 0, -1), 0.5, metallic(grey, 0.5)));
  // ground 
  world.add(sphere(point3(0,-100.5,-1), 100, metallic(light_green)));

  camera cam(/*camera_centre_=*/point3(0,0,1), /*vfov_=*/90);
  cam.set_max_depth(50);
  cam.set_samples_per_pixel(10);

  cam.render(world);

  return 0;
}
