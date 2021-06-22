#include <gtest/gtest.h>
#include <renderer/patterns/checker.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Checker, creating_a_checker_pattern)
{
	Patterns::Checker pattern = Patterns::Checker(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorA(), Graphics::Color::white());
	EXPECT_EQ(pattern.colorB(), Graphics::Color::black());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Checker, checker_pattern_repeat_x)
{
	Patterns::Checker pattern = Patterns::Checker(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0.99, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 1.01, 0, 0 }), Graphics::Color::black());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Checker, checker_pattern_repeat_y)
{
	Patterns::Checker pattern = Patterns::Checker(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 0.99, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 1.01, 0 }), Graphics::Color::black());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Checker, checker_pattern_repeat_z)
{
	Patterns::Checker pattern = Patterns::Checker(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0,0,0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0,0,0.99 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0,0,1.01 }), Graphics::Color::black());
}
