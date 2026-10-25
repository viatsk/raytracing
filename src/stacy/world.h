#ifndef WORLD_H
#define WORLD_H

// Eventually sphere should be replaced with
// triangles
#include "sphere.h"
#include "hit_record.h"

#include <cmath>
#include <vector>

class world {
 public:
  world() {}

  // Add and remove objects
  void add(sphere object) {
    objects_.push_back(object);
    bounding_box = box_union(bounding_box, object.bounding_box());
    std::cerr << "BOX: " << bounding_box << "\n";
  }
  
  void clear() { objects_.clear(); }

  // Rendering the world requires hitting anything
  std::optional<hit_record> hit(const ray& r, double closest_so_far) const {
    std::optional<hit_record> hit;
    uint num_hit_tests = 0;
    auto closest = closest_so_far;

    for (const sphere& object : objects_) {
      std::optional<hit_record> tmp_rec = object.hit(r, closest);
      if (tmp_rec.has_value()) {
        num_hit_tests++;
        closest = tmp_rec.value().t;
        tmp_rec.value().num_hit_tests = num_hit_tests;
        hit = tmp_rec;
      }
    }
  
    // TODO: Implement HIT function for bound box.
    // std::optional<hit_record> bounding_box_record = bounding_box.hit(r);
    // if (bounding_box_record.has_value()) {
    //   hit = bounding_box_record;
    // }
    
    return hit;
  }

 private:
  std::vector<sphere> objects_;
  bounds3 bounding_box;
};



#endif

