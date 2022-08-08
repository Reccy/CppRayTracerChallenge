#include <gtest/gtest.h>
#include <RML.h>
#include "math/sphere.h"
#include "math/ray.h"
#include "math/intersections.h"
#include "renderer/material.h"

namespace CppRayTracerChallenge::Core::Math
{
	TEST(CppRayTracerChallenge_Core_Math_Sphere, creating_a_sphere)
	{
		Sphere sphere = Sphere();

		EXPECT_EQ(sphere.transform(), Transform());
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, transforming_a_sphere)
	{
		Transform transform = Transform()
			.translate(2, 3, 4);

		Sphere sphere = Sphere();
		sphere.transform(transform);

		EXPECT_EQ(sphere.transform(), transform);
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, normal_at_point_on_x_axis)
	{
		Sphere sphere = Sphere();
		EXPECT_EQ(sphere.normal({ 1, 0, 0 }), Vector(1, 0, 0));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, normal_at_point_on_y_axis)
	{
		Sphere sphere = Sphere();
		EXPECT_EQ(sphere.normal({ 0, 1, 0 }), Vector(0, 1, 0));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, normal_at_point_on_z_axis)
	{
		Sphere sphere = Sphere();
		EXPECT_EQ(sphere.normal({ 0, 0, 1 }), Vector(0, 0, 1));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, normal_at_non_axial_point)
	{
		double x = sqrt(3) / 3;

		Sphere sphere = Sphere();
		EXPECT_EQ(sphere.normal({ x,x,x }), Vector(x, x, x));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, normal_vector_is_normalized)
	{
		double x = sqrt(3) / 3;

		Sphere sphere = Sphere();
		Vector normal = sphere.normal({ x,x,x });

		EXPECT_EQ(normal.normalize(), normal);
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, normal_on_translated_sphere)
	{
		Sphere sphere = Sphere();
		sphere.transform(Transform().translate(0, 1, 0));

		EXPECT_EQ(sphere.normal({ 0, 1.70711, -0.70711 }), Vector(0, 0.70711, -0.70711));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, normal_on_scaled_and_rotated_sphere)
	{
		Sphere sphere = Sphere();
		sphere.transform(Transform().rotate(0, 0, RML::Trig::PI / 5).scale(1, 0.5, 1));

		auto result = sphere.normal({ 0, sqrt(2) / 2, -sqrt(2) / 2 });
		auto expectedResult = Vector(0, 0.97014, -0.24254);

		EXPECT_EQ(result, expectedResult);
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, computing_a_ray_intersecting_a_sphere_at_two_points)
	{
		Ray ray = Ray(Point(0, 0, -5), Vector(0, 0, 1));
		Sphere sphere = Sphere();
		Intersections intersections = sphere.intersect(ray);

		EXPECT_EQ(intersections.size(), 2);
		EXPECT_EQ(intersections.at(0), Intersection(4, sphere));
		EXPECT_EQ(intersections.at(1), Intersection(6, sphere));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, computing_a_ray_intersecting_a_sphere_at_a_tangent)
	{
		Ray ray = Ray(Point(0, 1, -5), Vector(0, 0, 1));
		Sphere sphere = Sphere();
		Intersections intersections = sphere.intersect(ray);

		EXPECT_EQ(intersections.size(), 2);
		EXPECT_EQ(intersections.at(0), Intersection(5, sphere));
		EXPECT_EQ(intersections.at(1), Intersection(5, sphere));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, computing_a_ray_missing_a_sphere)
	{
		Ray ray = Ray(Point(0, 2, -5), Vector(0, 0, 1));
		Sphere sphere = Sphere();
		Intersections intersections = sphere.intersect(ray);

		EXPECT_EQ(intersections.size(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, ray_originates_inside_sphere)
	{
		Ray ray = Ray(Point(0, 0, 0), Vector(0, 0, 1));
		Sphere sphere = Sphere();
		Intersections intersections = sphere.intersect(ray);

		EXPECT_EQ(intersections.size(), 2);
		EXPECT_EQ(intersections.at(0), Intersection(-1, sphere));
		EXPECT_EQ(intersections.at(1), Intersection(1, sphere));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, ray_originates_in_front_of_sphere)
	{
		Ray ray = Ray(Point(0, 0, 5), Vector(0, 0, 1));
		Sphere sphere = Sphere();
		Intersections intersections = sphere.intersect(ray);

		EXPECT_EQ(intersections.size(), 2);
		EXPECT_EQ(intersections.at(0), Intersection(-6, sphere));
		EXPECT_EQ(intersections.at(1), Intersection(-4, sphere));
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, ray_intersection_sets_object)
	{
		Ray ray = Ray(Point(0, 0, -5), Vector(0, 0, 1));
		Sphere sphere = Sphere();
		Intersections intersections = sphere.intersect(ray);

		EXPECT_EQ(intersections.size(), 2);
		EXPECT_EQ(&intersections.at(0).shape(), &sphere);
		EXPECT_EQ(&intersections.at(0).shape(), &sphere);
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, intersecting_scaled_sphere_with_ray)
	{
		Ray ray = Ray(Point(0, 0, -5), Vector(0, 0, 1));
		Sphere sphere = Sphere();
		sphere.transform(Transform().scale(2, 2, 2));

		Intersections intersections = sphere.intersect(ray);

		EXPECT_EQ(intersections.count(), 2);
		EXPECT_EQ(intersections.at(0).t(), 3);
		EXPECT_EQ(intersections.at(1).t(), 7);
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, intersecting_translated_sphere_with_ray)
	{
		Ray ray = Ray(Point(0, 0, -5), Vector(0, 0, 1));
		Sphere sphere = Sphere();
		sphere.transform(Transform().translate(5, 0, 0));

		Intersections intersections = sphere.intersect(ray);

		EXPECT_EQ(intersections.count(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Math_Sphere, sphere_has_bounding_box)
	{
		Sphere sphere = Sphere();
		BoundingBox box = sphere.bounds();

		Point expectedMin = Point(-1, -1, -1);
		Point expectedMax = Point(1, 1, 1);

		EXPECT_EQ(box.min(), expectedMin);
		EXPECT_EQ(box.max(), expectedMax);
	}
}
