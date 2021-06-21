#include <gtest/gtest.h>
#include "renderer/shape.h"
#include "renderer/patterns/stripe.h"
#include "renderer/patterns/solid_color.h"
#include "renderer/patterns/test_pattern.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core;
using Renderer::Shape;
using Renderer::Material;
using Renderer::Patterns::SolidColor;
using Renderer::Patterns::Stripe;
using Renderer::Patterns::TestPattern;
using Math::Sphere;
using Math::Transform;
using Math::Point;
using Graphics::Color;

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_wraps_transform)
{
	auto sphere = std::make_shared<Sphere>(Sphere());
	Transform transform = Transform()
		.translate(2, 5, 20)
		.rotate(12, 30, 98);

	sphere->transform(transform);
	Shape shape = Shape(sphere);

	EXPECT_EQ(shape.transform(), sphere->transform());
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_wraps_normal)
{
	auto sphere = std::make_shared<Sphere>(Sphere());
	Shape shape = Shape(sphere);

	Point p = Point(2, 3, 4);

	EXPECT_EQ(sphere->normal(p), shape.normal(p));
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_wraps_normal_local)
{
	auto sphere = std::make_shared<Sphere>(Sphere());
	Shape shape = Shape(sphere);

	Point p = Point(2, 3, 4);

	EXPECT_EQ(sphere->normalLocal(p), shape.normalLocal(p));
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_has_a_default_material)
{
	auto sphere = std::make_shared<Sphere>();
	Shape shape = Shape(sphere);

	EXPECT_EQ(shape.material(), Material());
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_can_be_assigned_a_material)
{
	auto sphere = std::make_shared<Sphere>();
	Shape shape = Shape(sphere);

	Material material;
	material.pattern = std::make_shared<SolidColor>(Color(0.5, 0.5, 0.5));

	shape.material(material);

	EXPECT_EQ(shape.material(), material);
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_has_pattern_with_object_transform)
{
	auto sphere = std::make_shared<Sphere>();
	Shape shape = Shape(sphere);

	shape.transform(Transform().scale(2, 2, 2));

	Material material;
	material.pattern = std::make_shared<TestPattern>();
	shape.material(material);

	Color result = shape.colorAt({ 2, 3, 4 });
	Color expectedResult = Color(1, 1.5f, 2);

	EXPECT_EQ(result, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_has_pattern_with_pattern_transform)
{
	auto sphere = std::make_shared<Sphere>();
	Shape shape = Shape(sphere);

	Material material;
	material.pattern = std::make_shared<TestPattern>();
	material.pattern->transform(Transform().scale(2, 2, 2));
	shape.material(material);

	Color result = shape.colorAt({ 2, 3, 4 });
	Color expectedResult = Color(1, 1.5f, 2);

	EXPECT_EQ(result, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Renderer_Shape, shape_has_pattern_with_object_transform_and_pattern_transform)
{
	auto sphere = std::make_shared<Sphere>();
	Shape shape = Shape(sphere);

	Material material;
	material.pattern = std::make_shared<TestPattern>();
	material.pattern->transform(Transform().translate(0.5, 1, 1.5));
	shape.material(material);
	shape.transform(Transform().scale(2, 2, 2));

	Color result = shape.colorAt({ 2.5, 3, 3.5 });
	Color expectedResult = Color(0.75f, 0.5f, 0.25f);

	EXPECT_EQ(result, expectedResult);
}
