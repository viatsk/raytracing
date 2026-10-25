#include "stacy/bounds3.h"
#include <gtest/gtest.h>


TEST(Bounds3Test, UnionTwoBoxes){
  bounds3 unit_box = bounds3(point3(0, 0, 0), point3(1, 1, 1));
  bounds3 second_box = bounds3(point3(2, 2, 2), point3(4, 5, 6));

  ASSERT_EQ(
    box_union(second_box, unit_box),
    bounds3(point3(0, 0, 0), point3(4, 5, 6)));
}


