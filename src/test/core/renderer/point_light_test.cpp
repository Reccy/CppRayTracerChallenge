#include <gtest/gtest.h>
#include "renderer/point_light.h"
#include "graphics/color.h"
#include "math/point.h"

using namespace CppRayTracerChallenge::Core::Renderer;

TEST(CppRayTracerChallenge_Core_Renderer_PointLight, point_light_has_position_and_intensity)
{
	Graphics::Color intensity = Graphics::Color::white();
	Math::Point position = Math::Point(0, 0, 0);

	PointLight light = PointLight(position, intensity);

	EXPECT_EQ(light.position(), position);
	EXPECT_EQ(light.intensity(), intensity);
}
