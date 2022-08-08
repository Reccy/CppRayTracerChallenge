#include <gtest/gtest.h>
#include <renderer/patterns/checker.h>

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Checker, creating_a_checker_pattern)
	{
		Checker pattern = Checker(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorA(), Graphics::Color::white());
		EXPECT_EQ(pattern.colorB(), Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Checker, checker_pattern_repeat_x)
	{
		Checker pattern = Checker(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0.99, 0, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 1.01, 0, 0 }), Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Checker, checker_pattern_repeat_y)
	{
		Checker pattern = Checker(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0, 0.99, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0, 1.01, 0 }), Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Checker, checker_pattern_repeat_z)
	{
		Checker pattern = Checker(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorAt({ 0,0,0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0,0,0.99 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0,0,1.01 }), Graphics::Color::black());
	}

}
