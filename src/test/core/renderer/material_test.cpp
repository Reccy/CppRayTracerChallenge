#include <gtest/gtest.h>
#include "renderer/material.h"
#include "graphics/color.h"

using namespace CppRayTracerChallenge::Core::Renderer;

TEST(CppRayTracerChallenge_Core_Renderer_Material, default_material)
{
	Material material;

	EXPECT_EQ(material.color, Graphics::Color::white());
	EXPECT_EQ(material.ambient, 0.1);
	EXPECT_EQ(material.diffuse, 0.9);
	EXPECT_EQ(material.specular, 0.9);
	EXPECT_EQ(material.shininess, 200.0);
}
