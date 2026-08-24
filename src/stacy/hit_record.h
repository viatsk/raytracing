#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "colour.h"
#include "ray.h"
#include "vec3.h"

struct hit_record {
  point3 p;
  vec3 normal;
  double t;
  colour attenuation;
  ray scattered;
  bool inner_face; // refracted ray
};

#endif
