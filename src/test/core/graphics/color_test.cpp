#include <gtest/gtest.h>
#include "graphics/color.h"

using namespace CppRayTracerChallenge::Core::Graphics;

TEST(CppRayTracerChallenge_Core_Graphics_Color, red)
{
	Color color(0.5f, 0.4f, 1.7f);

	float expectedResult = 0.5f;

	EXPECT_EQ(color.red(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Graphics_Color, green)
{
	Color color(0.5f, 0.4f, 1.7f);

	float expectedResult = 0.4f;

	EXPECT_EQ(color.green(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Graphics_Color, blue)
{
	Color color(0.5f, 0.4f, 1.7f);

	float expectedResult = 1.7f;

	EXPECT_EQ(color.blue(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Graphics_Color, adding_colors)
{
	Color colorA(0.9f, 0.6f, 0.75f);
	Color colorB(0.7f, 0.1f, 0.25f);

	Color expectedResult(1.6f, 0.7f, 1.0f);

	EXPECT_EQ(colorA + colorB, expectedResult);
}
