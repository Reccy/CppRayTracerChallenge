#include <gtest/gtest.h>
#include "renderer/lighting.h"
#include "renderer/material.h"
#include "math/point.h"
#include "math/vector.h"

using namespace CppRayTracerChallenge::Core;

class CppRayTracerChallenge_Core_Renderer_LightingTest : public ::testing::Test
{
protected:
	Renderer::Material* material = nullptr;
	Math::Point* position = nullptr;

	virtual void SetUp()
	{
		material = new Renderer::Material();
		position = new Math::Point(0,0,0);
	}

	virtual void TearDown()
	{
		delete material;
		delete position;
	}
};

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_eye_between_light_and_surface)
{
	Math::Vector eyev = Math::Vector(0, 0, -1);
	Math::Vector normalv = Math::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*material, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(1.9f, 1.9f, 1.9f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_eye_between_light_and_surface_eye_offset_45_deg)
{
	Math::Vector eyev = Math::Vector(0, sqrt(2)/2, sqrt(2)/2);
	Math::Vector normalv = Math::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*material, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(1, 1, 1);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_eye_opposite_surface_light_offset_45_deg)
{
	Math::Vector eyev = Math::Vector(0, 0, -1);
	Math::Vector normalv = Math::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 10, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*material, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(0.7364f, 0.7364f, 0.7364f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_eye_in_the_path_of_reflection_vector)
{
	Math::Vector eyev = Math::Vector(0, -sqrt(2) / 2, -sqrt(2) / 2);
	Math::Vector normalv = Math::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 10, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*material, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(1.6364f, 1.6364f, 1.6364f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_the_light_behind_surface)
{
	Math::Vector eyev = Math::Vector(0, 0, -1);
	Math::Vector normalv = Math::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, 10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*material, light, *position, eyev, normalv, false);
	Graphics::Color expectedResult = Graphics::Color(0.1f, 0.1f, 0.1f);

	EXPECT_EQ(result, expectedResult);
}

TEST_F(CppRayTracerChallenge_Core_Renderer_LightingTest, lighting_with_the_surface_in_shadow)
{
	Math::Vector eyev = Math::Vector(0, 0, -1);
	Math::Vector normalv = Math::Vector(0, 0, -1);
	Renderer::PointLight light = Renderer::PointLight({ 0, 0, -10 }, { 1, 1, 1 });

	Graphics::Color result = Renderer::Lighting::lighting(*material, light, *position, eyev, normalv, true);
	Graphics::Color expectedResult = Graphics::Color(0.1f, 0.1f, 0.1f);

	EXPECT_EQ(result, expectedResult);
}
