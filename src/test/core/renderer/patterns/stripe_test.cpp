#include <gtest/gtest.h>
#include <renderer/patterns/stripe.h>

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, creating_a_stripe_pattern)
	{
		Stripe pattern = Stripe(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorA(), Graphics::Color::white());
		EXPECT_EQ(pattern.colorB(), Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, stripe_pattern_is_constant_in_y)
	{
		Stripe pattern = Stripe(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0, 1, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0, 2, 0 }), Graphics::Color::white());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, stripe_pattern_is_constant_in_z)
	{
		Stripe pattern = Stripe(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0, 0, 1 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0, 0, 2 }), Graphics::Color::white());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, stripe_pattern_alternates_in_x)
	{
		Stripe pattern = Stripe(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 0.9, 0, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 1, 0, 0 }), Graphics::Color::black());
		EXPECT_EQ(pattern.colorAt({ -0.1, 0, 0 }), Graphics::Color::black());
		EXPECT_EQ(pattern.colorAt({ -1, 0, 0 }), Graphics::Color::black());
		EXPECT_EQ(pattern.colorAt({ -1.1, 0, 0 }), Graphics::Color::white());
	}
}
