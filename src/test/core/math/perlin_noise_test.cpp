#include <gtest/gtest.h>
#include "math/perlin_noise.h"

namespace CppRayTracerChallenge::Core::Math
{
	TEST(CppRayTracerChallenge_Core_Math_PerlinNoise, create_perlin_noise_default)
	{
		PerlinNoise noise = PerlinNoise();
		EXPECT_EQ(noise.at({ 0, 0, 0 }), 0.5);
	}
}
