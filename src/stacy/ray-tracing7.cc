#include "camera.h"
#include "sphere.h"
#include "world.h"

#include <memory>
#include <iostream>

int main() {

  world world;
  world.add(sphere(point3(0, 0, -1), 0.5));
  world.add(sphere(point3(-1, 0, -1), 0.5));
  world.add(sphere(point3(1, 0, -1), 0.5));
  world.add(sphere(point3(0,-100.5,-1), 100));

  camera cam(/*camera_centre_=*/point3(0,0,1), /*vfov_=*/90);
  cam.set_using_shading(true);

  cam.render(world);

  return 0;
}


