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
