#include <gtest/gtest.h>
#include <renderer/patterns/stripe.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, creating_a_stripe_pattern)
{
	Patterns::Stripe pattern = Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorA(), Graphics::Color::white());
	EXPECT_EQ(pattern.colorB(), Graphics::Color::black());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, stripe_pattern_is_constant_in_y)
{
	Patterns::Stripe pattern = Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 1, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 2, 0 }), Graphics::Color::white());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, stripe_pattern_is_constant_in_z)
{
	Patterns::Stripe pattern = Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 0, 1 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0, 0, 2 }), Graphics::Color::white());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Stripe, stripe_pattern_alternates_in_x)
{
	Patterns::Stripe pattern = Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black());

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 0.9, 0, 0 }), Graphics::Color::white());
	EXPECT_EQ(pattern.colorAt({ 1, 0, 0 }), Graphics::Color::black());
	EXPECT_EQ(pattern.colorAt({ -0.1, 0, 0 }), Graphics::Color::black());
	EXPECT_EQ(pattern.colorAt({ -1, 0, 0 }), Graphics::Color::black());
	EXPECT_EQ(pattern.colorAt({ -1.1, 0, 0 }), Graphics::Color::white());
}

