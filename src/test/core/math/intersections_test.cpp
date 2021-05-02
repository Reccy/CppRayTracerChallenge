#include <gtest/gtest.h>
#include "math/intersections.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Intersections, creates_a_composition_of_intersections)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(2.0, obj);
	Intersection i2 = Intersection(3.0, obj);

	Intersections intersections = Intersections({ i1, i2 });

	EXPECT_EQ(intersections.count(), 2);
}
