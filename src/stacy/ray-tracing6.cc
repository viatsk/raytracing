#include "camera.h"
#include "sphere.h"
#include "world.h"

#include <memory>
#include <iostream>


int main() {
  colour light_green = colour(0.565, 0.933, 0.565);

  // World
  world world;
  world.add(std::make_unique<sphere>(point3(0, 0, -1), 0.5, red));
  world.add(std::make_unique<sphere>(point3(-1, 0, -1), 0.5, black));
  world.add(std::make_unique<sphere>(point3(1, 0, -1), 0.5, blue));
  world.add(std::make_unique<sphere>(point3(0,-100.5,-1), 100, light_green));

  camera cam(/*camera_centre_=*/point3(2,3,1), /*vfov_=*/70);
  cam.render(world);

  return 0;
}


