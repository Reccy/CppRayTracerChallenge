#include <gtest/gtest.h>
#include "math/perlin_noise.h"

using CppRayTracerChallenge::Core::Math::PerlinNoise;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Math_PerlinNoise, create_perlin_noise_default)
{
	PerlinNoise noise = PerlinNoise();
	EXPECT_EQ(noise.at({ 0, 0, 0 }), 0.5);
}
