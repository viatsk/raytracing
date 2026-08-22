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

// Based on artistic preference
static colour light_blue = colour(0.5, 0.7, 1.0);
static colour light_green = colour(0.565, 0.933, 0.565);
static colour light_red = colour(0.85, 0.3, 0.3);
static colour grey = colour(0.5, 0.5, 0.5);

void write_colour(std::ostream& out, const colour& pixel_colour) {
  auto r = pixel_colour.x();
  auto g = pixel_colour.y();
  auto b = pixel_colour.z();

  auto linear_to_gamma = [](double v) {
    return (v > 0) ? std::sqrt(v) : 0;
  };

  auto clamp = [](double x) -> double {
    if (x < 0.000) return 0.000;
    if (x > 0.999) return 0.999;
    return x;
  };

  // Translate [0, 1] components into the byte range [0, 255]
  int ir = int(255.999 * clamp(linear_to_gamma(r)));
  int ig = int(255.999 * clamp(linear_to_gamma(g)));
  int ib = int(255.999 * clamp(linear_to_gamma(b)));
  
  // Write out PPM format
  std::cout << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif
