#include <gtest/gtest.h>
#include "math/intersection.h"
#include "math/i_intersectable.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_IntersectionTest, creates_intersection)
{
	double t = 3.5;
	Sphere obj = Sphere();
	Intersection intersection = Intersection(t, obj);

	EXPECT_EQ(intersection.t(), t);
	EXPECT_EQ(&intersection.intersectable(), &obj);
}
