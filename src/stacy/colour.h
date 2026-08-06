#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"

#include <iostream>

using colour = vec3;

// Standard colours
static colour red = colour(1.0, 0.0, 0.0);
static colour green = colour(0.0, 1.0, 0.0);
static colour blue = colour(0.0, 0.0, 1.0);
static colour white = colour(1.0, 1.0, 1.0);
static colour black = colour(0.0, 0.0, 0.0);

void write_colour(std::ostream& out, const colour& pixel_colour) {
  auto r = pixel_colour.x();
  auto g = pixel_colour.y();
  auto b = pixel_colour.z();

  // Translate [0, 1] components into the byte range [0, 255]
  int ir = int(255.999 * r);
  int ig = int(255.999 * g);
  int ib = int(255.999 * b);
  
  // Write out PPM format
  std::cout << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif
