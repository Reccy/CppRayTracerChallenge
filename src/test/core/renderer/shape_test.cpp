#include <gtest/gtest.h>
#include "renderer/shape.h"
#include "renderer/patterns/solid_color.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_wraps_transform)
{
	auto sphere = std::make_shared<Math::Sphere>(Math::Sphere());
	Math::Transform transform = Math::Transform()
		.translate(2, 5, 20)
		.rotate(12, 30, 98);

	sphere->transform(transform);
	Renderer::Shape shape = Renderer::Shape(sphere);

	EXPECT_EQ(shape.transform(), sphere->transform());
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_wraps_normal)
{
	auto sphere = std::make_shared<Math::Sphere>(Math::Sphere());
	Renderer::Shape shape = Renderer::Shape(sphere);

	Math::Point p = Math::Point(2, 3, 4);

	EXPECT_EQ(sphere->normal(p), shape.normal(p));
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_wraps_normal_local)
{
	auto sphere = std::make_shared<Math::Sphere>(Math::Sphere());
	Renderer::Shape shape = Renderer::Shape(sphere);

	Math::Point p = Math::Point(2, 3, 4);

	EXPECT_EQ(sphere->normalLocal(p), shape.normalLocal(p));
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_has_a_default_material)
{
	auto sphere = std::make_shared<Math::Sphere>(Math::Sphere());
	Renderer::Shape shape = Renderer::Shape(sphere);

	EXPECT_EQ(shape.material(), Renderer::Material());
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_can_be_assigned_a_material)
{
	auto sphere = std::make_shared<Math::Sphere>(Math::Sphere());
	Renderer::Shape shape = Renderer::Shape(sphere);

	Renderer::Material material;
	material.pattern = std::make_shared<Renderer::Patterns::SolidColor>(Renderer::Patterns::SolidColor(Graphics::Color(0.5, 0.5, 0.5)));

	shape.material(material);

	EXPECT_EQ(shape.material(), material);
}
