#include "colour.h"
#include "vec3.h"

#include <iostream>

int main() {
  // Image
  int img_width = 256;
  int img_height = 256;

  // Render
  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

  for (int h = 0; h < img_height; h++) {
    std::clog << "\r Scanlines remaining: " << (img_height - h) <<  " " << std::flush;
      for (int w = 0; w < img_width; w++) {
        vec3 pixel_colour = colour(double(w)/(img_width-1), 0, double(h)/(img_height-1));
        write_colour(std::cout, pixel_colour);
    }
  }

  std::clog << "\r Done! \n";
  return 0;
}


