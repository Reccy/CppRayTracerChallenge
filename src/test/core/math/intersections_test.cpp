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

TEST(CppRayTracerChallenge_Core_Math_Intersections, hit_when_all_intersections_have_positive_t)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(1, obj);
	Intersection i2 = Intersection(2, obj);
	Intersections intersections = Intersections({ i1, i2 });

	EXPECT_EQ(*intersections.hit(), intersections.at(0));
}

TEST(CppRayTracerChallenge_Core_Math_Intersections, hit_when_some_intersections_have_negative_t)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(-1, obj);
	Intersection i2 = Intersection(1, obj);
	Intersections intersections = Intersections({ i1, i2 });

	EXPECT_EQ(*intersections.hit(), intersections.at(1));
}

TEST(CppRayTracerChallenge_Core_Math_Intersections, hit_when_all_intersections_have_negative_t)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(-1, obj);
	Intersection i2 = Intersection(-2, obj);
	Intersections intersections = Intersections({ i1, i2 });

	EXPECT_EQ(intersections.hit(), std::nullopt);
}

TEST(CppRayTracerChallenge_Core_Math_Intersection, hit_is_always_the_lowest_non_negative_intersection)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(5, obj);
	Intersection i2 = Intersection(7, obj);
	Intersection i3 = Intersection(-3, obj);
	Intersection i4 = Intersection(2, obj);
	Intersections intersections = Intersections({ i1, i2, i3, i4 });

	EXPECT_EQ(*intersections.hit(), intersections.at(3));
}
