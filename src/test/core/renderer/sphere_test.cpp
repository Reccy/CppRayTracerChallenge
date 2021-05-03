#include <gtest/gtest.h>
#include "renderer/sphere.h"

using namespace CppRayTracerChallenge::Core::Renderer;

TEST(CppRayTracerChallenge_Core_Renderer_Sphere, sphere_has_a_default_material)
{
	Sphere sphere = Sphere();
	EXPECT_EQ(sphere.material(), Renderer::Material());
}

TEST(CppRayTracerChallenge_Core_Renderer_Sphere, sphere_can_be_assigned_a_material)
{
	Sphere sphere = Sphere();

	Renderer::Material material;
	material.color = Graphics::Color(0.5, 0.5, 0.5);

	sphere.material(material);

	EXPECT_EQ(sphere.material(), material);
}
