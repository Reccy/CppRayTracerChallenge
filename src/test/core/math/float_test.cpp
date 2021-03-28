#include "gtest/gtest.h"
#include "math/float.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Float, compare_exact_values)
{
	float floatA = 1.0f;
	float floatB = 1.0f;
	EXPECT_TRUE(Float::compare(floatA, floatB));
}

TEST(CppRayTracerChallenge_Core_Math_Float, compare_values_inside_epsilon)
{
	float floatA = 1.0f;
	float floatB = 1.000001f;
	EXPECT_TRUE(Float::compare(floatA, floatB));
}

TEST(CppRayTracerChallenge_Core_Math_Float, compare_values_outside_epsilon)
{
	float floatA = 1.0f;
	float floatB = 1.00001f;
	EXPECT_FALSE(Float::compare(floatA, floatB));
}
