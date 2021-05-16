#include <gtest/gtest.h>
#include "renderer/object.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Object, object_has_a_default_material)
{
	Math::Sphere sphere = Math::Sphere();
	Renderer::Object obj = Renderer::Object(sphere);

	EXPECT_EQ(obj.material(), Renderer::Material());
}

TEST(CppRayTracerChallenge_Core_Renderer_Object, object_can_be_assigned_a_material)
{
	Math::Sphere sphere = Math::Sphere();
	Renderer::Object obj = Renderer::Object(sphere);

	Renderer::Material material;
	material.color = Graphics::Color(0.5, 0.5, 0.5);

	obj.material(material);

	EXPECT_EQ(obj.material(), material);
}
