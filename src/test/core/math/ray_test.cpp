#include <gtest/gtest.h>
#include "math/ray.h"
#include "math/point.h"
#include "math/vector.h"
#include "math/sphere.h"
#include "math/intersections.h"

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

TEST(CppRayTracerChallenge_Core_Math_Ray, computing_a_ray_intersecting_a_sphere_at_two_points)
{
	Ray ray = Ray(Point(0, 0, -5), Vector(0, 0, 1));
	Sphere sphere = Sphere();
	Intersections intersections = ray.intersect_sphere(sphere);

	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0), Intersection(4, sphere));
	EXPECT_EQ(intersections.at(1), Intersection(6, sphere));
}

TEST(CppRayTracerChallenge_Core_Math_Ray, computing_a_ray_intersecting_a_sphere_at_a_tangent)
{
	Ray ray = Ray(Point(0, 1, -5), Vector(0, 0, 1));
	Sphere sphere = Sphere();
	Intersections intersections = ray.intersect_sphere(sphere);

	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0), Intersection(5, sphere));
	EXPECT_EQ(intersections.at(1), Intersection(5, sphere));
}

TEST(CppRayTracerChallenge_Core_Math_Ray, computing_a_ray_missing_a_sphere)
{
	Ray ray = Ray(Point(0, 2, -5), Vector(0, 0, 1));
	Sphere sphere = Sphere();
	Intersections intersections = ray.intersect_sphere(sphere);

	EXPECT_EQ(intersections.size(), 0);
}

TEST(CppRayTracerChallenge_Core_Math_Ray, ray_originates_inside_sphere)
{
	Ray ray = Ray(Point(0, 0, 0), Vector(0, 0, 1));
	Sphere sphere = Sphere();
	Intersections intersections = ray.intersect_sphere(sphere);

	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0), Intersection(-1, sphere));
	EXPECT_EQ(intersections.at(1), Intersection(1, sphere));
}

TEST(CppRayTracerChallenge_Core_Math_Ray, ray_originates_in_front_of_sphere)
{
	Ray ray = Ray(Point(0, 0, 5), Vector(0, 0, 1));
	Sphere sphere = Sphere();
	Intersections intersections = ray.intersect_sphere(sphere);

	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0), Intersection(-6, sphere));
	EXPECT_EQ(intersections.at(1), Intersection(-4, sphere));
}

TEST(CppRayTracerChallenge_Core_Math_Ray, ray_intersection_sets_object)
{
	Ray ray = Ray(Point(0, 0, -5), Vector(0, 0, 1));
	Sphere sphere = Sphere();
	Intersections intersections = ray.intersect_sphere(sphere);

	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(&intersections.at(0).intersectable(), &sphere);
	EXPECT_EQ(&intersections.at(0).intersectable(), &sphere);
}
