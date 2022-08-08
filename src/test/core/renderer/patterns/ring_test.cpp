#include <gtest/gtest.h>
#include "renderer/patterns/ring.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Ring, creating_a_ring_pattern)
	{
		Ring pattern = Ring(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorA(), Graphics::Color::white());
		EXPECT_EQ(pattern.colorB(), Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Ring, ring_pattern)
	{
		Ring pattern = Ring(Graphics::Color::white(), Graphics::Color::black());

		EXPECT_EQ(pattern.colorAt({ 0, 0, 0 }), Graphics::Color::white());
		EXPECT_EQ(pattern.colorAt({ 1,0,0 }), Graphics::Color::black());
		EXPECT_EQ(pattern.colorAt({ 0,0,1 }), Graphics::Color::black());

		// 0.708 = just slightly more than sqrt(2)/2
		EXPECT_EQ(pattern.colorAt({ 0.708,0,0.708 }), Graphics::Color::black());
	}
}
