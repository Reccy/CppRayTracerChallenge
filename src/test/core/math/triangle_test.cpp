#include <gtest/gtest.h>
#include <math/triangle.h>

namespace CppRayTracerChallenge::Core::Math
{
	TEST(CppRayTracerChallenge_Core_Math_Triangle, constructing_a_triangle)
	{
		Point p1 = Point(0, 1, 0);
		Point p2 = Point(-1, 0, 0);
		Point p3 = Point(1, 0, 0);
		Triangle triangle = Triangle(p1, p2, p3);

		EXPECT_EQ(triangle.p1(), p1);
		EXPECT_EQ(triangle.p2(), p2);
		EXPECT_EQ(triangle.p3(), p3);

		EXPECT_EQ(triangle.e1(), Vector(-1, -1, 0));
		EXPECT_EQ(triangle.e2(), Vector(1, -1, 0));

		EXPECT_EQ(triangle.normal(), Vector(0, 0, -1));
	}

	TEST(CppRayTracerChallenge_Core_Math_Triangle, normal_on_triangle)
	{
		Triangle triangle = Triangle({ 0, 1, 0 }, { -1, 0, 0 }, { 1, 0, 0 });
		Vector n1 = triangle.normalLocal({ 0, 0.5, 0 });
		Vector n2 = triangle.normalLocal({ -0.5, 0.75, 0 });
		Vector n3 = triangle.normalLocal({ 0.5, 0.25, 0 });

		EXPECT_EQ(n1, triangle.normal());
		EXPECT_EQ(n2, triangle.normal());
		EXPECT_EQ(n3, triangle.normal());
	}

	TEST(CppRayTracerChallenge_Core_Math_Triangle, intersecting_ray_parallel_to_triangle)
	{
		Triangle triangle = Triangle({ 0, 1, 0 }, { -1, 0, 0 }, { 1, 0, 0 });
		Ray ray = Ray({ 0, -1, -2 }, { 0, 1, 0 });

		Intersections intersections = triangle.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Math_Triangle, ray_misses_p1_to_p3_edge)
	{
		Triangle triangle = Triangle({ 0, 1, 0 }, { -1, 0, 0 }, { 1, 0, 0 });
		Ray ray = Ray({ 1, 1, -2 }, { 0, 0, 1 });

		Intersections intersections = triangle.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Math_Triangle, ray_misses_p1_to_p2_edge)
	{
		Triangle triangle = Triangle({ 0, 1, 0 }, { -1, 0, 0 }, { 1, 0, 0 });
		Ray ray = Ray({ -1, 1, -2 }, { 0, 0, 1 });

		Intersections intersections = triangle.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Math_Triangle, ray_misses_p2_to_p3_edge)
	{
		Triangle triangle = Triangle({ 0, 1, 0 }, { -1, 0, 0 }, { 1, 0, 0 });
		Ray ray = Ray({ 0, -1, -2 }, { 0, 0, 1 });

		Intersections intersections = triangle.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Math_Triangle, ray_hits_triangle)
	{
		Triangle triangle = Triangle({ 0, 1, 0 }, { -1, 0, 0 }, { 1, 0, 0 });
		Ray ray = Ray({ 0, 0.5, -2 }, { 0, 0, 1 });

		Intersections intersections = triangle.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 1);
		EXPECT_EQ(intersections.at(0).t(), 2);
	}

	TEST(CppRayTracerChallenge_Core_Math_Triangle, bounding_box)
	{
		Point p1 = Point(-3, 7, 2);
		Point p2 = Point(6, 2, -4);
		Point p3 = Point(2, -1, -1);
		Triangle triangle = Triangle(p1, p2, p3);

		BoundingBox box = triangle.bounds();

		EXPECT_EQ(box.min(), Point(-3, -1, -4));
		EXPECT_EQ(box.max(), Point(6, 7, 2));
	}
}
