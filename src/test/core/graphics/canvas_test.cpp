#include <gtest/gtest.h>
#include "graphics/canvas.h"

using namespace CppRayTracerChallenge::Core::Graphics;

TEST(CppRayTracerChallenge_Core_Graphics_Canvas, construct_with_set_width)
{
	Canvas canvas(10, 20);

	EXPECT_EQ(canvas.width(), 10);
}

TEST(CppRayTracerChallenge_Core_Graphics_Canvas, construct_with_set_height)
{
	Canvas canvas(10, 20);

	EXPECT_EQ(canvas.height(), 20);
}

TEST(CppRayTracerChallenge_Core_Graphics_Canvas, construct_will_set_all_colors_to_black)
{
	Canvas canvas(3, 3);

	std::vector<Color> expectedResult{
		Color(0,0,0), Color(0,0,0), Color(0,0,0),
		Color(0,0,0), Color(0,0,0), Color(0,0,0),
		Color(0,0,0), Color(0,0,0), Color(0,0,0)
	};

	EXPECT_EQ(canvas.toBuffer(), expectedResult);
}
