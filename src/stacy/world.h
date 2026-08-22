#ifndef WORLD_H
#define WORLD_H

// List of hittables.
#include "sphere.h"

#include <concepts>
#include <cmath>
#include <vector>
#include <variant>

template<typename T>
concept HasHittableInterface = requires(T t) {
    { t.hit(std::declval<const ray&>(), std::declval<double>()) } -> std::same_as<std::optional<hit_record>>;
};

class world {
 public:
  // Enforce that hittable variant have hit implemented
  template<HasHittableInterface... Ts>
  using hittable_varient = std::variant<Ts...>;

  using hittable = hittable_varient<sphere>;

  world() {}

  // Add and remove objects
  void add(hittable object) {
    objects_.push_back(std::move(object));
  }
  
  void clear() { objects_.clear(); }

  // hittable requires hitting anything
  std::optional<hit_record> hit(const ray& r, double closest_so_far) const {
    std::optional<hit_record> hit;
    auto closest = closest_so_far;

    for (const hittable& object : objects_) {
      std::optional<hit_record> tmp_rec = std::visit(
        [r, closest] (auto&& object) { 
          return object.hit(r, closest); 
        }, object);
      if (tmp_rec.has_value()) {
        closest = tmp_rec.value().t;
        hit = tmp_rec;
      }
    }
  
    return hit;
  }

 private:
  std::vector<hittable> objects_;
};



#endif

