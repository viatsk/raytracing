#include "stacy/bounds3.h"
#include <gtest/gtest.h>


TEST(Bounds3Test, UnionTwoBoxes){
  bounds3 unit_box = bounds3(point3(0, 0, 0), point3(1, 1, 1));
  bounds3 second_box = bounds3(point3(2, 2, 2), point3(4, 5, 6));

  ASSERT_EQ(
    box_union(second_box, unit_box),
    bounds3(point3(0, 0, 0), point3(4, 5, 6)));
}

TEST(Bounds3Test, Centroid){
  bounds3 random_box = bounds3(point3(-1, 13, 7), point3(4, 5, 6));
  ASSERT_EQ(random_box.centroid(), point3(1.5, 9, 6.5));
}

TEST(Bounds3Test, MaximumExtent){
  bounds3 random_box = bounds3(point3(-1, 13, 7), point3(4, 5, 6));
  ASSERT_EQ(random_box.max_extent(), 1);
}

TEST(Bounds3Test, OffsetInsideBox) {
  point3 point1 = point3(-1, 13, 7);
  point3 point2 = point3(4, 5, 6);
  bounds3 bbox = bounds3(point1, point2);
  ASSERT_EQ(bbox.offset(point1), point3(0, 1, 1));
  ASSERT_EQ(bbox.offset(point2), point3(1, 0, 0));
}

TEST(Bounds3Test, OffsetOusideBox) {
  point3 point1 = point3(-1, 13, 7);
  point3 point2 = point3(4, 5, 6);
  bounds3 bbox = bounds3(point1, point2);

  point3 point_outside_box = point3(1000, 1000, 1000);
  ASSERT_EQ(bbox.offset(point_outside_box), point3(200.2, 124.375, 994));
}


