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

TEST(CppRayTracerChallenge_Core_Math_Intersections, creates_a_composition_of_intersections_that_are_sorted)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(3.0, obj);
	Intersection i2 = Intersection(2.0, obj);

	Intersections intersections = Intersections({ i1, i2 });

	EXPECT_EQ(intersections.count(), 2);
	EXPECT_EQ(intersections.at(0).t(), 2.0);
	EXPECT_EQ(intersections.at(1).t(), 3.0);
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

TEST(CppRayTracerChallenge_Core_Math_Intersections, hit_is_always_the_lowest_non_negative_intersection)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(5, obj);
	Intersection i2 = Intersection(7, obj);
	Intersection i3 = Intersection(-3, obj);
	Intersection i4 = Intersection(2, obj);
	Intersections intersections = Intersections({ i1, i2, i3, i4 });

	EXPECT_EQ(*intersections.hit(), i4);
}

TEST(CppRayTracerChallenge_Core_Math_Intersections, adding_intersections_sorts_in_order)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(5, obj);
	Intersection i2 = Intersection(7, obj);
	Intersection i3 = Intersection(-3, obj);
	Intersection i4 = Intersection(2, obj);

	Intersections a = Intersections({ i1, i2 });
	Intersections b = Intersections({ i3, i4 });

	Intersections c = a + b;

	EXPECT_EQ(c.at(0).t(), -3);
	EXPECT_EQ(c.at(1).t(), 2);
	EXPECT_EQ(c.at(2).t(), 5);
	EXPECT_EQ(c.at(3).t(), 7);
}

TEST(CppRayTracerChallenge_Core_Math_Intersections, assignment_adding_intersections_sorts_in_order)
{
	Sphere obj = Sphere();
	Intersection i1 = Intersection(5, obj);
	Intersection i2 = Intersection(7, obj);
	Intersection i3 = Intersection(-3, obj);
	Intersection i4 = Intersection(2, obj);

	Intersections a = Intersections({ i1, i2 });
	Intersections b = Intersections({ i3, i4 });

	a += b;

	EXPECT_EQ(a.at(0).t(), -3);
	EXPECT_EQ(a.at(1).t(), 2);
	EXPECT_EQ(a.at(2).t(), 5);
	EXPECT_EQ(a.at(3).t(), 7);
}
