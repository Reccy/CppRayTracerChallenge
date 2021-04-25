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
