#include <gtest/gtest.h>
#include "math/intersection.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Intersection, creates_intersection)
{
	double t = 3.5;
	Sphere obj = Sphere();
	Intersection intersection = Intersection(t, obj);

	EXPECT_EQ(intersection.t(), t);
	EXPECT_EQ(&intersection.intersectable(), &obj);
}

TEST(CppRayTracerChallenge_Core_Math_Intersection, equality)
{
	double t = 3.5;
	Sphere obj = Sphere();
	Intersection i1 = Intersection(t, obj);
	Intersection i2 = Intersection(t, obj);

	EXPECT_TRUE(i1 == i2);
}

TEST(CppRayTracerChallenge_Core_Math_Intersection, inequality_t)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(2.5, obj);
	Intersection i2 = Intersection(3.5, obj);

	EXPECT_TRUE(i1 != i2);
}

TEST(CppRayTracerChallenge_Core_Math_Intersection, inequality_intersectable)
{
	Sphere obj1 = Sphere();
	Sphere obj2 = Sphere();
	double t = 3.5;

	Intersection i1 = Intersection(t, obj1);
	Intersection i2 = Intersection(t, obj2);

	EXPECT_TRUE(i1 != i2);
}
