#include <gtest/gtest.h>
#include <graphics/color.h>
#include <renderer/material.h>
#include <renderer/pattern.h>
#include <renderer/patterns/solid_color.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Material, default_material)
{
	Material material;

	EXPECT_EQ(typeid(*material.pattern), typeid(Patterns::SolidColor));
	EXPECT_EQ(material.ambient, 0.1f);
	EXPECT_EQ(material.diffuse, 0.9f);
	EXPECT_EQ(material.specular, 0.9f);
	EXPECT_EQ(material.shininess, 200.0f);
}
