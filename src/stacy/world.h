#ifndef WORLD_H
#define WORLD_H

#include "hittable.h"

#include <cmath>
#include <memory>
#include <vector>

class world : public hittable {
 public:
  world() {}

  // Add and remove objects
  void add(std::unique_ptr<hittable> object) {
    objects_.push_back(std::move(object));
  }
  
  void clear() { objects_.clear(); }

  // hittable requires hitting anything
  std::optional<hit_record> hit(const ray& r, double closest_so_far) const override {
    std::optional<hit_record> hit;
    auto closest = closest_so_far;

    for (const std::unique_ptr<hittable>& object : objects_) {
      std::optional<hit_record> tmp_rec;
      tmp_rec = object->hit(r, closest);
      if (tmp_rec.has_value()) {
        closest = tmp_rec.value().t;
        hit = tmp_rec;
      }
    }
  
    return hit;
  }

 private:
  std::vector<std::unique_ptr<hittable>> objects_;
};



#endif

