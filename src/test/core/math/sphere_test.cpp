#include <gtest/gtest.h>
#include "math/sphere.h"
#include "math/vector.h"
#include "math/trig.h"
#include "renderer/material.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Sphere, creating_a_sphere)
{
	Sphere sphere = Sphere();

	EXPECT_EQ(sphere.position(), Point(0, 0, 0));
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
	sphere.transform(Transform().rotate(0, 0, Trig::PI/5).scale(1, 0.5, 1));

	EXPECT_EQ(sphere.normal({ 0, sqrt(2) / 2, -sqrt(2) / 2 }), Vector(0, 0.97014, -0.24254));
}
