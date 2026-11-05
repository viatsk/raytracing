#ifndef BVH_H
#define BVH_H

#include "bounds3.h"
#include "sphere.h"

#include <memory>

constexpr int kNumBuckets = 4;

// Return union of all bounding boxes in list
static bounds3 bbox_list(const std::vector<sphere>& list) {
  return std::accumulate(list.begin(), list.end(),
        bounds3 {},[](bounds3 box, const sphere& s) {
                  return box_union(box, s.bounding_box());
              });
}

// Split Method: Surface Area Heuristic.
class bvh {
 public:
  bvh(std::vector<sphere> list) : bvh(list, 0, list.size(), bbox_list(list)) {} 
  bvh(std::vector<sphere> list, size_t start, size_t end, const bounds3 bbox) {
    std::cerr << "bvh recursive: ";
    // Step 0: Return early
    int num_primatives = end - start;
    if (num_primatives == 1) {
      // TODO: Enforce being the same as list[start] and list[end] box?
      bbox_ = bbox;
      std::cerr << "Leaf! \n";
    }
    
    // Step 2: Chose the axis associated w/ the largest extent when projecting
    // the bounding box centroid for the curent set of primitives.
    // We choose the axis with the largest range of bbox centroids.
    bounds3 centroid_bounds;
    for (int i = start; i < end; ++i) {
      centroid_bounds = box_union(centroid_bounds, list[i].bounding_box().centroid());
    }
    std::cerr << " centroid bounds are " << centroid_bounds << "\n";
    int split_dim = centroid_bounds.max_extent();
    std::cerr << "split dim is " << split_dim;
    
    // Step 3: Arrange everything into buckets
    bounds3 buckets[kNumBuckets];
    int bucket_counts[kNumBuckets] = {};
    std::cerr << "\n";
    for (int i = start; i < end; ++i){
      bounds3 prim_box = list[i].bounding_box();
      std::cerr << "prim_box.centroid" << prim_box.centroid() << " ";
      std::cerr << "centroid_offset " << centroid_bounds.offset(prim_box.centroid()) << "\t";
      int bucket_index = (kNumBuckets-1)* (centroid_bounds.offset(prim_box.centroid())[split_dim]);
      std::cerr << "bucket index  " << bucket_index << "\t";
      // bucket_index = std::min(bucket_index, kNumBuckets - 1);
      buckets[bucket_index] = box_union(buckets[bucket_index], prim_box);
      bucket_counts[bucket_index]++;
      std::cerr << " in box " << buckets[bucket_index] << " w/ sa " << prim_box.surface_area() << "\n";
    }


  
    // Step 4: Estimate the cost of splitting at each bucket boundary
    bounds3 prefix_bounds[kNumBuckets];
    int left_counts[kNumBuckets] = {};
    prefix_bounds[0] = buckets[0];
    left_counts[0] = bucket_counts[0];
    std::cerr << "\nPrefix bbox[0]: " << prefix_bounds[0] << 
        " surface area is " << left_counts[0];
    for (int i = 1; i < kNumBuckets; ++i) {
      prefix_bounds[i] = box_union(prefix_bounds[i-1], buckets[i]);
      left_counts[i] = left_counts[i-1] + bucket_counts[i];
      std::cerr << "\nPrefix bbox[" << i << "]: " << prefix_bounds[i] << 
        " surface area is " << left_counts[i];
    }

    bounds3 suffix_bounds[kNumBuckets];
    int right_counts[kNumBuckets] = {};
    suffix_bounds[kNumBuckets - 1] = buckets[kNumBuckets - 1];
    right_counts[kNumBuckets-1] = bucket_counts[kNumBuckets-1];
    std::cerr << "\nSuffix bbox[3]: " << suffix_bounds[kNumBuckets - 1] <<
           " surface area is " << right_counts[kNumBuckets - 1];
    for (int i = kNumBuckets - 2; i >= 0; --i) {
      suffix_bounds[i] = box_union(suffix_bounds[i+1], buckets[i]);
      right_counts[i] = right_counts[i+1] + bucket_counts[i];
      std::cerr << "\nSuffix bbox[" << i << "]: " << suffix_bounds[i] <<
           " surface area is " << right_counts[i];
    }

    // Step 5: Choose the best stuff
    float min_cost = INFINITY;
    int min_cost_split = 0;
    std::cerr << "\n";
    for (int i = 0; i < kNumBuckets-1; ++i) {
      std::cerr << "left box: " << prefix_bounds[i] << "right box " << suffix_bounds[i+1];
      std::cerr << " Split plane: " << prefix_bounds[i].max_[split_dim] << " same as " << suffix_bounds[i+1].min_[split_dim];
      float cost = 0.125 + ((left_counts[i] * prefix_bounds[i].surface_area() + right_counts[i+1] * suffix_bounds[i+1].surface_area()) / bbox.surface_area());
      std::cerr << " cost is " << cost << "\n";
      if (cost < min_cost) {
        min_cost = cost;
        min_cost_split = i;
      }
    }

    std::cerr << "Min split cost: " << buckets[min_cost_split];
    // auto mid = std::partition(
    // list.begin() + start,
    // list.begin() + end,
    // [&](const Primitive& prim) {
    //     return prim.bounding_box().centroid()[split_dim] < split_pos;
    // }
    // );

    std::cerr << "\n";
  }

  bounds3 left_box() {
    return left_->bounding_box();
  }
  bounds3 right_box() {
    return right_->bounding_box();
  }

  bool hit(const ray& r) {
    // if (!bbox.hit(r))
    //   return false;
    // bool hit_left = left->hit(r);
    // bool hit_right = right->hit(r);

    // return hit_left || hit_right;
    return false;
  }

  bounds3 bounding_box() {return bbox_; }

 private:
  std::unique_ptr<bvh> left_;
  std::unique_ptr<bvh> right_;
  // TODO:  storing a variable-sized array of primitive indices at each leaf node.
  bounds3 bbox_;
};


#endif
