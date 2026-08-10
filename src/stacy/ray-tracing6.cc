#include "camera.h"
#include "sphere.h"
#include "world.h"

#include <memory>
#include <iostream>

int main() {

  // World
  world world;
  world.add(std::make_unique<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_unique<sphere>(point3(-1, 0, -1), 0.5));
  world.add(std::make_unique<sphere>(point3(1, 0, -1), 0.5));
  world.add(std::make_unique<sphere>(point3(0,-100.5,-1), 100));

  camera cam;
  cam.render(world);

  return 0;
}


