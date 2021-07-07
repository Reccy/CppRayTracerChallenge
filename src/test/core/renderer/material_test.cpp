#include <gtest/gtest.h>
#include <graphics/color.h>
#include <renderer/material.h>
#include <renderer/pattern.h>
#include <renderer/patterns/solid_color.h>
#include "helpers/material_helper.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Material, default_material)
{
	Material material;

	EXPECT_EQ(*material.pattern, Patterns::SolidColor(Graphics::Color::white()));
	EXPECT_EQ(material.ambient, 0.1f);
	EXPECT_EQ(material.diffuse, 0.9f);
	EXPECT_EQ(material.specular, 0.9f);
	EXPECT_EQ(material.shininess, 200.0f);
	EXPECT_EQ(material.reflective, 0.0f);
	EXPECT_EQ(material.transparency, 0.0f);
	EXPECT_EQ(material.refractiveIndex, 1.0f);
}
