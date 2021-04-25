#include <gtest/gtest.h>
#include "math/ray.h"
#include "math/point.h"
#include "math/vector.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Ray, creating_and_querying_ray)
{
	Point origin = Point(1, 2, 3);
	Vector direction = Vector(4, 5, 6);

	Ray ray = Ray(origin, direction);

	EXPECT_EQ(ray.origin(), origin);
	EXPECT_EQ(ray.direction(), direction);
}

TEST(CppRayTracerChallenge_Core_Math_Ray, computing_a_point_from_a_distance)
{
	Ray ray = Ray(Point(2, 3, 4), Vector(1, 0, 0));

	EXPECT_EQ(ray.position(0), Point(2, 3, 4));
	EXPECT_EQ(ray.position(1), Point(3, 3, 4));
	EXPECT_EQ(ray.position(-1), Point(1, 3, 4));
	EXPECT_EQ(ray.position(2.5), Point(4.5, 3, 4));
}
