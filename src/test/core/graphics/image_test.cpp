#include <gtest/gtest.h>
#include "graphics/image.h"

using namespace CppRayTracerChallenge::Core::Graphics;


TEST(CppRayTracerChallenge_Core_Graphics_Image, construct_with_set_width)
{
	Image image(5, 10);

	EXPECT_EQ(image.width(), 5);
}

TEST(CppRayTracerChallenge_Core_Graphics_Image, construct_with_set_height)
{
	Image image(5, 10);

	EXPECT_EQ(image.height(), 10);
}

TEST(CppRayTracerChallenge_Core_Graphics_Image, construct_will_set_all_colors_to_black)
{
	Image image(3, 3);

	std::vector<std::vector<Color>> expectedResult{
		std::vector<Color> { Color::black(), Color::black(), Color::black() },
		std::vector<Color> { Color::black(), Color::black(), Color::black() },
		std::vector<Color> { Color::black(), Color::black(), Color::black() }
	};

	EXPECT_EQ(image.toBuffer(), expectedResult);
}
