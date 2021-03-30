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

TEST(CppRayTracerChallenge_Core_Math_Float, sqrt)
{
	float floatA = 14.0f;
	
	float result = Float::sqrt(floatA);
	float expectedResult = 3.74165738677f;

	EXPECT_EQ(result, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Float, pow)
{
	float base = 10.0f;
	float power = 2.0f;

	float result = Float::pow(base, power);
	float expectedResult = 100.0f;

	EXPECT_EQ(result, expectedResult);
}
