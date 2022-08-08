#include <gtest/gtest.h>
#include <RML.h>
#include "math/plane.h"

namespace CppRayTracerChallenge::Core::Math
{
	using RML::INF;

	TEST(CppRayTracerChallenge_Core_Math_Plane, normal_is_constant)
	{
		Plane plane = Plane();

		Vector n1 = plane.normalLocal({ 0, 0, 0 });
		Vector n2 = plane.normalLocal({ 10, 0, -10 });
		Vector n3 = plane.normalLocal({ -5, 0, 150 });

		EXPECT_EQ(n1, Vector::up());
		EXPECT_EQ(n2, Vector::up());
		EXPECT_EQ(n3, Vector::up());
	}

	TEST(CppRayTracerChallenge_Core_Math_Plane, intersect_is_parallel)
	{
		Plane plane = Plane();
		Ray ray = Ray({ 0, 10, 0 }, { 0, 0, 1 });

		Intersections intersections = plane.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Math_Plane, intersect_is_coplanar)
	{
		Plane plane = Plane();
		Ray ray = Ray({ 0,0,0 }, { 0,0,1 });

		Intersections intersections = plane.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Math_Plane, intersect_is_from_above)
	{
		Plane plane = Plane();
		Ray ray = Ray({ 0, 1, 0 }, { 0, -1, 0 });

		Intersections intersections = plane.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 1);
		EXPECT_EQ(intersections.hit().value().t(), 1);
		EXPECT_EQ(&intersections.hit().value().shape(), &plane);
	}

	TEST(CppRayTracerChallenge_Core_Math_Plane, intersect_is_from_below)
	{
		Plane plane = Plane();
		Ray ray = Ray({ 0, -1, 0 }, { 0, 1,0 });

		Intersections intersections = plane.intersectLocal(ray);

		EXPECT_EQ(intersections.count(), 1);
		EXPECT_EQ(intersections.hit().value().t(), 1);
		EXPECT_EQ(&intersections.hit().value().shape(), &plane);
	}

	TEST(CppRayTracerChallenge_Core_Math_Plane, plane_has_bounding_box)
	{
		Plane plane = Plane();
		BoundingBox box = plane.bounds();

		Point expectedMin = Point(-INF, 0, -INF);
		Point expectedMax = Point(INF, 0, INF);
	}
}
