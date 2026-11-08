#include "stacy/bvh.h"
#include <gtest/gtest.h>

#include<memory>

// Trivial case: Four flush spheres
TEST(BVHTest, BasicFourSpheres){
  sphere sphere0 = sphere(point3(-1, 0, 0), 0.5);
  sphere sphere1 = sphere(point3(0, 0, 0), 0.5);
  sphere sphere2 = sphere(point3(1, 0, 0), 0.5);
  sphere sphere3 = sphere(point3(2, 0, 0), 0.5);

  std::vector<sphere>spheres = {sphere0, sphere1, sphere2, sphere3};
  auto bound_volume_hierarchy = std::make_unique<bvh>(spheres);
  bound_volume_hierarchy->validate();
}

TEST(BVHTest, ThreeDimentionsTest){
  sphere sphere0 = sphere(point3(-1, 7, -6), 0.5);
  sphere sphere1 = sphere(point3(0, -5, 3), 0.5);
  sphere sphere2 = sphere(point3(1, 13, -12), 0.5);

  std::vector<sphere>spheres = {sphere0, sphere1, sphere2};
  auto bound_volume_hierarchy = std::make_unique<bvh>(spheres);
  bound_volume_hierarchy->validate();
}

// 900 randomly-distributed spheres produce a valid tree
TEST(BVHTest, SmallRandomSpheres) {
  std::vector<sphere> spheres;
  for (int a = -15; a < 15; a++) {
    for (int b = -15; b < 15; b++) {
      point3 centre(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
      spheres.push_back(sphere(centre, 0.2));
    }
  }
  auto bound_volume_hierarchy = std::make_unique<bvh>(spheres);
  bound_volume_hierarchy->validate();
}

TEST(BVHTest, ConcentricSpheres) {
  std::vector<sphere> spheres;
  point3 centre = point3(1,2, 3);
  spheres.push_back(sphere(centre, 0.6));
  spheres.push_back(sphere(centre, 0.5));

  auto bound_volume_hierarchy = std::make_unique<bvh>(spheres);
  bound_volume_hierarchy->validate();
}

