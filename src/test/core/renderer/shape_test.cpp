#include <gtest/gtest.h>
#include "renderer/shape.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core;

// todo add tests for wrapper

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_wraps_transform)
{
	Math::Sphere sphere = Math::Sphere();
	Math::Transform transform = Math::Transform()
		.translate(2, 5, 20)
		.rotate(12, 30, 98);

	sphere.transform(transform);

	Renderer::Shape shape = Renderer::Shape(sphere);

	EXPECT_EQ(shape.transform(), sphere.transform());
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_wraps_normal)
{
	Math::Sphere sphere = Math::Sphere();
	Renderer::Shape shape = Renderer::Shape(sphere);

	Math::Point p = Math::Point(2, 3, 4);

	EXPECT_EQ(sphere.normal(p), shape.normal(p));
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_has_a_default_material)
{
	Math::Sphere sphere = Math::Sphere();
	Renderer::Shape shape = Renderer::Shape(sphere);

	EXPECT_EQ(shape.material(), Renderer::Material());
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_can_be_assigned_a_material)
{
	Math::Sphere sphere = Math::Sphere();
	Renderer::Shape shape = Renderer::Shape(sphere);

	Renderer::Material material;
	material.color = Graphics::Color(0.5, 0.5, 0.5);

	shape.material(material);

	EXPECT_EQ(shape.material(), material);
}
