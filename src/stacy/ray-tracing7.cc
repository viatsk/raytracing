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
  world.add(std::make_unique<sphere>(point3(0,-100.5,-1), 100, light_green));

  camera cam(/*camera_centre_=*/point3(0,0,0), /*vfov_=*/90);
  cam.set_using_shading(true);

  cam.render(world);

  return 0;
}


