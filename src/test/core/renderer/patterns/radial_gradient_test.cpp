#include <gtest/gtest.h>
#include "renderer/patterns/radial_gradient.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_RadialGradient, creating_a_radial_gradient_pattern)
{
	Patterns::RadialGradient pattern = Patterns::RadialGradient(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorA(), Graphics::Color::white());
	EXPECT_EQ(pattern.colorB(), Graphics::Color::black());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_RadialGradient, gradiant_lerps_from_a_to_b_along_x)
{
	Patterns::RadialGradient pattern = Patterns::RadialGradient(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0.9999999, 0, 0 }), Graphics::Color::black());
	EXPECT_EQ(pattern.colorAt({ 0.5, 0, 0 }), Graphics::Color(0.5f, 0.5f, 0.5f));
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_RadialGradient, gradiant_lerps_from_a_to_b_along_x_and_wraps)
{
	Patterns::RadialGradient pattern = Patterns::RadialGradient(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 1.9999999, 0, 0 }), Graphics::Color::black());
	EXPECT_EQ(pattern.colorAt({ 1.5, 0, 0 }), Graphics::Color(0.5f, 0.5f, 0.5f));
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_RadialGradient, gradiant_lerps_from_a_to_b_along_z)
{
	Patterns::RadialGradient pattern = Patterns::RadialGradient(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 0, 0.9999999 }), Graphics::Color::black());
	EXPECT_EQ(pattern.colorAt({ 0, 0, 0.5 }), Graphics::Color(0.5f, 0.5f, 0.5f));
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_RadialGradient, gradiant_lerps_from_a_to_b_along_z_and_wraps)
{
	Patterns::RadialGradient pattern = Patterns::RadialGradient(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 1.9999999 }), Graphics::Color::black());
	EXPECT_EQ(pattern.colorAt({ 0, 0, 1.5 }), Graphics::Color(0.5f, 0.5f, 0.5f));
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_RadialGradient, gradiant_does_not_change_along_y)
{
	Patterns::RadialGradient pattern = Patterns::RadialGradient(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 0.9999999, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 0.5, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 1.9999999, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 1.5, 0 }), Graphics::Color::white());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_RadialGradient, gradiant_lerps_from_a_to_b_along_x_and_z)
{
	Patterns::RadialGradient pattern = Patterns::RadialGradient(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0.354, 0, 0.354 }), Graphics::Color(0.499368f, 0.499368f, 0.499368f));
	EXPECT_EQ(pattern.colorAt({ 0.708, 0, 0.708 }), Graphics::Color(0.998737f, 0.998737f, 0.998737f));
	EXPECT_EQ(pattern.colorAt({ 0.354 * 2, 0, 0.354 * 2 }), Graphics::Color(0.998737f, 0.998737f, 0.998737f));
}
