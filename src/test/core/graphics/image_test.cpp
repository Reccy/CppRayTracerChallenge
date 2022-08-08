#include <gtest/gtest.h>
#include "graphics/image.h"

namespace CppRayTracerChallenge::Core::Graphics
{
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

		std::vector<Color> expectedResult{
			Color::black(), Color::black(), Color::black(),
			Color::black(), Color::black(), Color::black(),
			Color::black(), Color::black(), Color::black()
		};

		EXPECT_EQ(image.toBuffer(), expectedResult);
	}

	TEST(CppRayTracerChallange_Core_Graphics_Image, construct_with_set_colors)
	{
		std::vector<Color> colors{
			Color::black(), Color::black(), Color::white(),
			Color::black(), Color::white(), Color::black(),
			Color::white(), Color::black(), Color::black()
		};

		std::vector<Color> expectedResult{
			Color::black(), Color::black(), Color::white(),
			Color::black(), Color::white(), Color::black(),
			Color::white(), Color::black(), Color::black()
		};

		Image image(3, 3, colors);

		EXPECT_EQ(image.toBuffer(), expectedResult);
	}

	TEST(CppRayTracerChallenge_Core_Graphics_Image, construct_with_single_color)
	{
		Image image(3, 3, Color::white());

		std::vector<Color> expectedResult{
			Color::white(), Color::white(), Color::white(),
			Color::white(), Color::white(), Color::white(),
			Color::white(), Color::white(), Color::white()
		};

		EXPECT_EQ(image.toBuffer(), expectedResult);
	}
}
