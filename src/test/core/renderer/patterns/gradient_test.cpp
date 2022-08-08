#include <gtest/gtest.h>
#include "renderer/patterns/gradient.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	using Graphics::Color;

	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Gradient, gradient_lerps_from_a_to_b)
	{
		auto pattern = Gradient(Color::white(), Color::black());

		EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Color::white());
		EXPECT_EQ(pattern.colorAt({ 0.25, 0, 0 }), Color(0.75f, 0.75f, 0.75f));
		EXPECT_EQ(pattern.colorAt({ 0.5,0.5,0.5 }), Color(0.5f, 0.5f, 0.5f));
		EXPECT_EQ(pattern.colorAt({ 0.75,0,0 }), Color(0.25f, 0.25f, 0.25f));
	}
}
