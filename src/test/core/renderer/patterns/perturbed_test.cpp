#include <gtest/gtest.h>
#include "renderer/pattern.h"
#include "renderer/patterns/perturbed.h"
#include "renderer/patterns/solid_color.h"
#include "graphics/color.h"

using CppRayTracerChallenge::Core::Renderer::Pattern;
using namespace CppRayTracerChallenge::Core::Renderer::Patterns;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Patterns_Perturbed, create_perturbed_by_wrapping_solid_color_pattern)
{
	std::shared_ptr<Pattern> solidColorPtr = std::make_shared<SolidColor>(Graphics::Color::white());
	Perturbed pattern = Perturbed(solidColorPtr);

	EXPECT_EQ(pattern.colorAt({ 0,0,0 }), Graphics::Color::white());
}
