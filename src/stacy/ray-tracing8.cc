#include "camera.h"
#include "sphere.h"
#include "world.h"

#include <iostream>

int main() {

  world world;
  world.add(sphere(point3(0, 0, -1), 0.5, metallic(light_blue)));
  world.add(sphere(point3(-1, 0, -1), 0.5, metallic(grey)));
  world.add(sphere(point3(1, 0, -1), 0.5, metallic(light_red)));
  world.add(sphere(point3(0,-100.5,-1), 100, metallic(light_green)));

  camera cam(/*camera_centre_=*/point3(3,2,1), /*vfov_=*/40);

  cam.render(world);

  return 0;
}


