#include <gtest/gtest.h>
#include <renderer/patterns/masked.h>
#include <renderer/patterns/solid_color.h>
#include <renderer/patterns/checker.h>
#include <graphics/color.h>
#include <math/transform.h>

using CppRayTracerChallenge::Core::Renderer::Patterns::Masked;
using CppRayTracerChallenge::Core::Renderer::Patterns::Checker;
using CppRayTracerChallenge::Core::Renderer::Patterns::SolidColor;
using CppRayTracerChallenge::Core::Graphics::Color;
using CppRayTracerChallenge::Core::Math::Transform;

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Masked, can_create_masked_pattern_with_default_transform)
{
	std::shared_ptr<SolidColor> a = std::make_shared<SolidColor>(Color::white());
	std::shared_ptr<SolidColor> b = std::make_shared<SolidColor>(Color::black());

	Masked<Checker> pattern = Masked<Checker>(a, b);
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Masked, can_create_masked_pattern_with_custom_transform)
{
	std::shared_ptr<SolidColor> a = std::make_shared<SolidColor>(Color::white());
	std::shared_ptr<SolidColor> b = std::make_shared<SolidColor>(Color::black());

	Transform transform = Transform().scale(2, 1, 2);

	Masked<Checker> pattern = Masked<Checker>(a, b, transform);
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Masked, can_render_different_patterns)
{
	std::shared_ptr<SolidColor> a = std::make_shared<SolidColor>(Color::white());
	std::shared_ptr<SolidColor> b = std::make_shared<SolidColor>(Color::black());

	Transform transform = Transform().scale(2, 1, 2);

	Masked<Checker> pattern = Masked<Checker>(a, b, transform);

	EXPECT_EQ(pattern.colorAt({0, 0, 0}), Color::white());
	EXPECT_EQ(pattern.colorAt({1, 0, 0}), Color::black());
}

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Masked, can_render_different_patterns_and_transform_mask)
{
	std::shared_ptr<SolidColor> a = std::make_shared<SolidColor>(Color::white());
	std::shared_ptr<SolidColor> b = std::make_shared<SolidColor>(Color::black());

	Transform transform = Transform().scale(2, 1, 2);

	Masked<Checker> pattern = Masked<Checker>(a, b, transform);

	EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Color::white());
	EXPECT_EQ(pattern.colorAt({ 0.999999, 0, 0 }), Color::white());
	EXPECT_EQ(pattern.colorAt({ 1.999999, 0, 0 }), Color::black());
}
