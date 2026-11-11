#ifndef WORLD_H
#define WORLD_H

// Eventually sphere should be replaced with
// triangles
#include "bvh.h"
#include "sphere.h"
#include "hit_record.h"

#include <cmath>
#include <vector>
#include <memory>

class world {
 public:
  world() {}

  void add(sphere object) {
    objects_.push_back(object);
  }  
  void clear() { objects_.clear(); }

  // TODO: Clean up / make interdependant
  void render_aabbs(bool render) { debug_render_boxes_ = render; }
  void make_bvh() {
    bound_volume_hierarchy_ = std::make_unique<bvh>(objects_);
    bound_volume_hierarchy_->validate();
  }

  // Rendering the world requires hitting anything
  std::optional<hit_record> hit(const ray& r, double closest_so_far) const {
    std::optional<hit_record> hit;
    uint num_hit_tests = 0;
    auto closest = closest_so_far;

    if (bound_volume_hierarchy_) {
      std::optional<hit_record> tmp_rec = bound_volume_hierarchy_->hit(r, closest);
      if (tmp_rec.has_value()) {
        closest = tmp_rec.value().t;
        hit = tmp_rec;
      }
    } else {
      for (const sphere& object : objects_) {
        num_hit_tests++;
        std::optional<hit_record> tmp_rec = object.hit(r, closest);
        if (tmp_rec.has_value()) {
          closest = tmp_rec.value().t;
          tmp_rec.value().num_hit_tests = num_hit_tests;
          hit = tmp_rec;
        }
      }
    }

    // TODO: Implement a second renderpass to visualize boxes,
    // currently things look Bad.
    if (debug_render_boxes_) {
      std::optional<hit_record> bounding_box_record = bound_volume_hierarchy_->hit_with_colour_info(r, closest, 0);
      if (bounding_box_record.has_value()) {
        hit = bounding_box_record;
      }
    }
    
    return hit;
  }

 private:
  std::vector<sphere> objects_;
  std::unique_ptr<bvh> bound_volume_hierarchy_;
  bool debug_render_boxes_ = false; 
};



#endif

