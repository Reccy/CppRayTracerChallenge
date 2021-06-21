#include <gtest/gtest.h>
#include "renderer/patterns/stripe.h"
#include "renderer/patterns/solid_color.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_equality_true)
{
	auto a = std::make_unique<Patterns::Stripe>(Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black()));
	auto b = std::make_unique<Patterns::Stripe>(Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black()));

	EXPECT_TRUE(*a == *b);
}

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_equality_false)
{
	std::unique_ptr<Pattern> a = std::make_unique<Patterns::Stripe>(Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black()));
	std::unique_ptr<Pattern> b = std::make_unique<Patterns::SolidColor>(Patterns::SolidColor(Graphics::Color::white()));

	EXPECT_FALSE(*a == *b);
}

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_inequality_true)
{
	auto a = std::make_unique<Patterns::Stripe>(Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black()));
	auto b = std::make_unique<Patterns::SolidColor>(Patterns::SolidColor(Graphics::Color::white()));

	EXPECT_TRUE(*a != *b);
}

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_inequality_false)
{
	std::unique_ptr<Pattern> a = std::make_unique<Patterns::Stripe>(Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black()));
	std::unique_ptr<Pattern> b = std::make_unique<Patterns::Stripe>(Patterns::Stripe(Graphics::Color::white(), Graphics::Color::black()));

	EXPECT_FALSE(*a != *b);
}
