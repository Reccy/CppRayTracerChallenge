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

	std::vector<Color> expectedResult {
		Color::black(), Color::black(), Color::black(),
		Color::black(), Color::black(), Color::black(),
		Color::black(), Color::black(), Color::black()
	};

	EXPECT_EQ(canvas.toBuffer(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Graphics_Canvas, write_pixel_will_update_color_in_canvas)
{
	Canvas canvas(3, 3);

	canvas.writePixel(0, 0, Color::white());

	EXPECT_EQ(canvas.readPixel(0, 0), Color::white());
}

TEST(CppRayTracerChallenge_Core_Graphics_Canvas, write_pixel_will_update_color_in_canvas_with_buffer)
{
	Canvas canvas(3, 3);

	canvas.writePixel(0, 0, Color::white());
	canvas.writePixel(0, 1, Color::white());
	canvas.writePixel(0, 2, Color::white());
	canvas.writePixel(1, 0, Color::white());
	canvas.writePixel(2, 0, Color::white());

	std::vector<Color> expectedResult {
		Color::white(), Color::white(), Color::white(),
		Color::white(), Color::black(), Color::black(),
		Color::white(), Color::black(), Color::black()
	};

	EXPECT_EQ(canvas.toBuffer(), expectedResult);
}
