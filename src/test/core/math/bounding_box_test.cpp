#include <gtest/gtest.h>
#include <math/constants.h>
#include <math/bounding_box.h>

using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Math_BoundingBox, create_empty_bounding_box)
{
	BoundingBox box = BoundingBox();

	Point expectedMin = Point({ INF, INF, INF });
	Point expectedMax = Point({ -INF, -INF, -INF });

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}

TEST(CppRayTracerChallenge_Core_Math_BoundingBox, create_with_volume)
{
	BoundingBox box = BoundingBox({ -1, -2, -3 }, { 3,2,1 });

	Point expectedMin = Point(-1, -2, -3);
	Point expectedMax = Point(3, 2, 1);

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}

TEST(CppRayTracerChallenge_Core_Math_BoundingBox, add_points_to_empty_bounding_box)
{
	BoundingBox box = BoundingBox();

	Point p1 = Point(-5, 2, 0);
	Point p2 = Point(7, 0, -3);

	box.add(p1);
	box.add(p2);

	Point expectedMin = Point(-5, 0, -3);
	Point expectedMax = Point(7, 2, 0);

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}
