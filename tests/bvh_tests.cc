#include "stacy/bvh.h"
#include <gtest/gtest.h>

#include<memory>

// Trivial case: Four flush spheres
TEST(BVHTest, FirstTest){
  sphere sphere0 = sphere(point3(-1, 0, 0), 0.5);
  sphere sphere1 = sphere(point3(0, 0, 0), 0.5);
  sphere sphere2 = sphere(point3(1, 0, 0), 0.5);
  sphere sphere3 = sphere(point3(2, 0, 0), 0.5);

  std::vector<sphere>spheres = {sphere0, sphere1, sphere2, sphere3};
  auto bound_volume_hierarchy = std::make_unique<bvh>(spheres);
  bound_volume_hierarchy->validate();
}

// Trivial case: Unbalanced tree
TEST(BVHTest, SplitSphereTest){
  sphere sphere0 = sphere(point3(-1, 7, -6), 0.5);
  sphere sphere1 = sphere(point3(0, -5, 3), 0.5);
  sphere sphere2 = sphere(point3(1, 13, -12), 0.5);

  std::vector<sphere>spheres = {sphere0, sphere1, sphere2};
  auto bound_volume_hierarchy = std::make_unique<bvh>(spheres);
  bound_volume_hierarchy->validate();
}

// 900 randomly-distributed spheres produce a valid tree
TEST(BVHTest, SmallRandomSpheres) {
  std::vector<sphere> world;
  for (int a = -15; a < 15; a++) {
    for (int b = -15; b < 15; b++) {
      point3 centre(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
      world.push_back(sphere(centre, 0.2));
    }
  }
  auto bound_volume_hierarchy = std::make_unique<bvh>(world);
  bound_volume_hierarchy->validate();
}
