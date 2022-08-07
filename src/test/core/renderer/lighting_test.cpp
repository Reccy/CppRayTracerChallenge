#include <gtest/gtest.h>
#include "renderer/lighting.h"
#include "renderer/material.h"
#include "renderer/patterns/stripe.h"
#include "renderer/patterns/test_pattern.h"
#include "renderer/shape.h"
#include "math/sphere.h"
#include <RML.h>

using namespace CppRayTracerChallenge::Core;

class CppRayTracerChallenge_Core_Renderer_LightingTest : public ::testing::Test
{
protected:
	std::shared_ptr<Renderer::Material> material;
	std::shared_ptr<RML::Point> position;
	std::shared_ptr<Math::Sphere> sphere;
	std::shared_ptr<Renderer::Shape> shape;

	virtual void SetUp()
	{
		material = std::make_unique<Renderer::Material>();
		position = std::make_unique<RML::Point>(0,0,0);
		sphere = std::make_unique<Math::Sphere>();
		material->pattern = std::make_unique<Renderer::Patterns::TestPattern>();
		shape = std::make_unique<Renderer::Shape>(sphere);
	}
};

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_eye_between_light_and_surface)
{
	RML::Vector eyev = RML::Vector(0, 0, -1);
	RML::Vector normalv = RML::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*shape, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(1.9f, 1.9f, 1.9f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_eye_between_light_and_surface_eye_offset_45_deg)
{
	RML::Vector eyev = RML::Vector(0, sqrt(2)/2, sqrt(2)/2);
	RML::Vector normalv = RML::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*shape, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(1, 1, 1);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_eye_opposite_surface_light_offset_45_deg)
{
	RML::Vector eyev = RML::Vector(0, 0, -1);
	RML::Vector normalv = RML::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 10, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*shape, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(0.7364f, 0.7364f, 0.7364f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_eye_in_the_path_of_reflection_vector)
{
	RML::Vector eyev = RML::Vector(0, -sqrt(2) / 2, -sqrt(2) / 2);
	RML::Vector normalv = RML::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 10, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*shape, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(1.6364f, 1.6364f, 1.6364f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_the_light_behind_surface)
{
	RML::Vector eyev = RML::Vector(0, 0, -1);
	RML::Vector normalv = RML::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, 10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*shape, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(0.1f, 0.1f, 0.1f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_the_surface_in_shadow)
{
	RML::Vector eyev = RML::Vector(0, 0, -1);
	RML::Vector normalv = RML::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*shape, light, *position, eyev, normalv, true);
	Graphics::Color expectedResult = Graphics::Color(0.1f, 0.1f, 0.1f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_stripe_pattern)
{
	RML::Vector eyev = RML::Vector(0, 0, -1);
	RML::Vector normalv = RML::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, -10 }, { 1, 1, 1 });

	material->pattern = std::make_shared<Renderer::Patterns::Stripe>(Renderer::Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black()));
	material->ambient = 1;
	material->diffuse = 0;
	material->specular = 0;

	shape->material(*material);

	Graphics::Color resultA = Renderer::Lighting::lighting(*shape, light, RML::Point(0.9f, 0, 0), eyev, normalv, false);
	Graphics::Color expectedResultA = Graphics::Color::white();

	EXPECT_EQ(resultA, expectedResultA);

	Graphics::Color resultB = Renderer::Lighting::lighting(*shape, light, RML::Point(1.1f, 0, 0), eyev, normalv, false);
	Graphics::Color expectedResultB = Graphics::Color::black();

	EXPECT_EQ(resultB, expectedResultB);
}
