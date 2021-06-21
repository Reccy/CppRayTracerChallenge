#include <gtest/gtest.h>
#include <renderer/patterns/solid_color.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, creating_a_solid_color_pattern)
{
	Patterns::SolidColor pattern = Patterns::SolidColor(Graphics::Color::white());

	EXPECT_EQ(pattern.color(), Graphics::Color::white());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, solid_color_pattern_is_constant_in_all_axes)
{
	Patterns::SolidColor pattern = Patterns::SolidColor(Graphics::Color::white());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 1, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 2, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ -1, 2, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 2, 1 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 2, 2 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, -2, -2 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 1, 2, -2 }), Graphics::Color::white());
}
