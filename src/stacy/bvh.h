#ifndef BVH_H
#define BVH_H

#include "bounds3.h"
#include "sphere.h"

#include <memory>
#include <cassert>

constexpr int kNumBuckets = 4;
const std::vector<colour> debug_palette = {red, green, blue};

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
  bvh(std::vector<sphere> list) : bvh(list, bbox_list(list)) {} 
  bvh(std::vector<sphere> list, const bounds3 bbox) {
    bbox_ = bbox;
    // Step 0: Return early in the trivial case.
    // TODO: Each leaf should store K primitives
    if (list.size() == 1) {
      object_ = list[0];
      return;
    }
    
    // Step 2: Chose the axis associated w/ the largest extent when projecting
    // the bounding box centroid for the curent set of primitives.
    // We choose the axis with the largest range of bbox centroids.
    bounds3 centroid_bounds;
    for (int i = 0; i < list.size(); ++i) {
      centroid_bounds = box_union(centroid_bounds, list[i].bounding_box().centroid());
    }
    int split_dim = centroid_bounds.max_extent();

    // Step 3: Arrange everything into buckets
    bounds3 buckets[kNumBuckets];
    int bucket_counts[kNumBuckets] = {};
    for (int i = 0; i < list.size(); ++i){
      bounds3 prim_box = list[i].bounding_box();
      // std::cerr << "prim_box.centroid" << prim_box.centroid() << " ";
      // std::cerr << "centroid_offset " << centroid_bounds.offset(prim_box.centroid()) << "\t";
      int bucket_index = (kNumBuckets-1)* (centroid_bounds.offset(prim_box.centroid())[split_dim]);
      // std::cerr << "bucket index  " << bucket_index << "\t";
      // bucket_index = std::min(bucket_index, kNumBuckets - 1);
      buckets[bucket_index] = box_union(buckets[bucket_index], prim_box);
      bucket_counts[bucket_index]++;
      // std::cerr << " in box " << buckets[bucket_index] << " w/ sa " << prim_box.surface_area() << "\n";
    }
  
    // Step 4: Estimate the cost of splitting at each bucket boundary
    bounds3 prefix_bounds[kNumBuckets];
    int left_counts[kNumBuckets] = {};
    prefix_bounds[0] = buckets[0];
    left_counts[0] = bucket_counts[0];
    for (int i = 1; i < kNumBuckets; ++i) {
      prefix_bounds[i] = box_union(prefix_bounds[i-1], buckets[i]);
      left_counts[i] = left_counts[i-1] + bucket_counts[i];
      // std::cerr << "\nPrefix bbox[" << i << "]: " << prefix_bounds[i] << 
      //   " surface area is " << left_counts[i];
    }

    bounds3 suffix_bounds[kNumBuckets];
    int right_counts[kNumBuckets] = {};
    suffix_bounds[kNumBuckets - 1] = buckets[kNumBuckets - 1];
    right_counts[kNumBuckets-1] = bucket_counts[kNumBuckets-1];
    for (int i = kNumBuckets - 2; i >= 0; --i) {
      suffix_bounds[i] = box_union(suffix_bounds[i+1], buckets[i]);
      right_counts[i] = right_counts[i+1] + bucket_counts[i];
      // std::cerr << "\nSuffix bbox[" << i << "]: " << suffix_bounds[i] <<
      //      " surface area is " << right_counts[i];
    }

    // Step 5: Choose the best stuff
    float min_cost = INFINITY;
    int min_cost_split = 0;
    for (int i = 0; i < kNumBuckets-1; ++i) {
      // Fail immediately if split planes are not flush.
      // assert((prefix_bounds[i].max_[split_dim] == suffix_bounds[i+1].min_[split_dim]));
      float cost = 0.125 + ((left_counts[i] * prefix_bounds[i].surface_area() + right_counts[i+1] * suffix_bounds[i+1].surface_area()) / bbox.surface_area());
      if (cost < min_cost) {
        min_cost = cost;
        min_cost_split = i;
      }
    }

    // Step 6: Build valid sub-trees.
    std::vector<sphere>::iterator midpoint = std::partition(list.begin(), list.end(), [=] (const sphere s) {
      int b = (kNumBuckets-1)* (centroid_bounds.offset(s.bounding_box().centroid())[split_dim]);
      return b <= min_cost_split;
    });
    left_ = make_unique<bvh>(std::vector<sphere>(list.begin(), midpoint), prefix_bounds[min_cost_split]);
    right_ = make_unique<bvh>(std::vector<sphere>(midpoint, list.end()), suffix_bounds[min_cost_split+1]);
    return;
  }


  bounds3 bounding_box() {return bbox_; }

  // Walks the tree, validating that invariants are preserved.
  void validate() {
    if (object_.has_value()) {
      assert(left_  == nullptr);
      assert(left_ == right_);
      assert(object_.value().bounding_box() == bbox_);
      return;
    }
    assert(left_ != right_);
    assert(left_ != nullptr);
    assert(right_ != nullptr);
    assert(box_union(left_->bounding_box(), right_->bounding_box()) == bbox_);
  }

  // TODO: Implement HIT
  std::optional<hit_record> hit_with_colour_info(const ray& r, int depth) {
    std::optional<hit_record> rec;
    if (object_.has_value()) {
      // Leaf Node
      rec = bbox_.hit_with_colour_info(r);
      if (rec.has_value()){
        rec.value().attenuation = debug_palette[depth % debug_palette.size()];
      }
      return rec;
    }
    rec = bbox_.hit_with_colour_info(r);
    if (rec.has_value()) {
      rec.value().attenuation = debug_palette[depth % debug_palette.size()];
      return rec;
    }
    rec = left_->hit_with_colour_info(r, depth + 1);
    if (rec.has_value()) {
      return rec;
    }
    rec = right_->hit_with_colour_info(r, depth + 1);
    if (rec.has_value()) {
      return rec;
    }
    return {};
  }

 private:
  static void print(std::ostream& out, const bvh& bvh_node, int depth) {
    std::string indent(depth * 2, ' ');
    if (bvh_node.object_.has_value()) {
      out << indent << "Leaf: " << bvh_node.bbox_ << "\n"; 
    } else {
      if (bvh_node.left_) {
        out << indent << "  Left:\n";
        print(out, *bvh_node.left_, depth + 2);
      }
      if (bvh_node.right_) {
        out << indent << "  Right:\n";
        print(out, *bvh_node.right_, depth + 2);
      }
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const bvh& bvh_node);
  std::unique_ptr<bvh> left_;
  std::unique_ptr<bvh> right_;
  std::optional<sphere> object_;
  bounds3 bbox_;
  // Store split axis?
};

inline std::ostream& operator<<(std::ostream& out, const bvh& bvh_node) {
  out << "bvh: \n";
  bvh::print(out, bvh_node, 0);
  return out;
}

#endif
