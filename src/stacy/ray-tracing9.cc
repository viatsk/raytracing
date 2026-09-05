#include "camera.h"
#include "sphere.h"
#include "world.h"

#include <iostream>

int main() {

  world world;
  // world.add(sphere(point3(0, 0, -1), 0.5, metallic(light_blue)));
  // world.add(sphere(point3(-1, 0, -1), 0.5, metallic(grey)));
  world.add(sphere(point3(1, 0, -1), 0.5, dielectric(1.0 / 1.5)));
  world.add(sphere(point3(0,-100.5,-1), 100, metallic(light_green)));

  camera cam(/*camera_centre_=*/point3(0,0,1), /*vfov_=*/90);
  cam.set_max_depth(50);
  cam.set_samples_per_pixel(100);


  cam.render(world);

  return 0;
}
