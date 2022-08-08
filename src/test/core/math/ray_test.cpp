#include <gtest/gtest.h>
#include <RML.h>
#include "math/ray.h"
#include "math/sphere.h"
#include "math/intersections.h"

namespace CppRayTracerChallenge::Core::Math
{
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

	TEST(CppRayTracerChallenge_Core_Math_Ray, ray_translation)
	{
		Ray ray = Ray(Point(1, 2, 3), Vector(0, 1, 0));
		Transform transform = Transform()
			.translate(3, 4, 5);
		Ray transformedRay = ray.transform(transform.matrix());

		EXPECT_EQ(transformedRay.origin(), Point(4, 6, 8));
		EXPECT_EQ(transformedRay.direction(), Vector(0, 1, 0));
	}

	TEST(CppRayTracerChallenge_Core_Math_Ray, ray_scaling)
	{
		Ray ray = Ray(Point(1, 2, 3), Vector(0, 1, 0));
		Transform transform = Transform()
			.scale(2, 3, 4);
		Ray transformedRay = ray.transform(transform.matrix());

		EXPECT_EQ(transformedRay.origin(), Point(2, 6, 12));
		EXPECT_EQ(transformedRay.direction(), Vector(0, 3, 0));
	}
}
