#include <gtest/gtest.h>
#include "renderer/patterns/stripe.h"
#include "renderer/patterns/solid_color.h"
#include "renderer/patterns/test_pattern.h"

using CppRayTracerChallenge::Core::Graphics::Color;
using CppRayTracerChallenge::Core::Renderer::Pattern;
using CppRayTracerChallenge::Core::Renderer::Patterns::SolidColor;
using CppRayTracerChallenge::Core::Renderer::Patterns::Stripe;
using CppRayTracerChallenge::Core::Renderer::Patterns::TestPattern;
using RML::Transform;

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_has_default_identity_matrix_as_transform)
{
	auto pattern = std::make_shared<TestPattern>();

	EXPECT_EQ(pattern->transform(), Transform());
}

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_transform_can_be_changed)
{
	auto newTransform = Transform().translate(5, 10, -5).rotate(1,2,3).get_inverted();
	auto pattern = std::make_shared<TestPattern>();

	pattern->transform(newTransform);

	EXPECT_EQ(pattern->transform(), newTransform);
}

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_equality_true)
{
	auto a = std::make_unique<Stripe>(Color::white(), Color::black());
	auto b = std::make_unique<Stripe>(Color::white(), Color::black());

	EXPECT_TRUE(*a == *b);
}

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_equality_false)
{
	std::unique_ptr<Pattern> a = std::make_unique<Stripe>(Color::white(), Color::black());
	std::unique_ptr<Pattern> b = std::make_unique<SolidColor>(Color::white());

	EXPECT_FALSE(*a == *b);
}

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_inequality_true)
{
	auto a = std::make_unique<Stripe>(Color::white(), Color::black());
	auto b = std::make_unique<SolidColor>(Color::white());

	EXPECT_TRUE(*a != *b);
}

TEST(CppRayTracerChallenge_Core_Renderer_Pattern, pattern_inequality_false)
{
	std::unique_ptr<Pattern> a = std::make_unique<Stripe>(Color::white(), Color::black());
	std::unique_ptr<Pattern> b = std::make_unique<Stripe>(Color::white(), Color::black());

	EXPECT_FALSE(*a != *b);
}
