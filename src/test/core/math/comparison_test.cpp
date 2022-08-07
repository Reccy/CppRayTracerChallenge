#include "gtest/gtest.h"
#include <RML.h>
#include "math/comparison.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Comparison, compare_exact_values_double)
{
	double doubleA = 1.0;
	double doubleB = 1.0;
	EXPECT_TRUE(Comparison::equal(doubleA, doubleB));
}

TEST(CppRayTracerChallenge_Core_Math_Comparison, compare_values_inside_epsilon_double)
{
	double doubleA = 1.0;
	double doubleB = 1.000001;
	EXPECT_TRUE(Comparison::equal(doubleA, doubleB));
}

TEST(CppRayTracerChallenge_Core_Math_Comparison, compare_values_outside_epsilon_double)
{
	double doubleA = 1.0;
	double doubleB = 1.00001f;
	EXPECT_FALSE(Comparison::equal(doubleA, doubleB));
}

TEST(CppRayTracerChallenge_Core_Math_Comparison, compare_exact_values_float)
{
	float floatA = 1.0f;
	float floatB = 1.0f;
	EXPECT_TRUE(Comparison::equal(floatA, floatB));
}

TEST(CppRayTracerChallenge_Core_Math_Comparison, compare_values_inside_epsilon_float)
{
	float floatA = 1.0f;
	float floatB = 1.000001f;
	EXPECT_TRUE(Comparison::equal(floatA, floatB));
}

TEST(CppRayTracerChallenge_Core_Math_Comparison, compare_values_outside_epsilon_float)
{
	float floatA = 1.0f;
	float floatB = 1.00001f;
	EXPECT_FALSE(Comparison::equal(floatA, floatB));
}

TEST(CppRayTracerChallenge_Core_Math_Comparison, compare_infinity)
{
	EXPECT_TRUE(Comparison::equal(RML::INF, RML::INF));
	EXPECT_TRUE(Comparison::equal(-RML::INF, -RML::INF));
	EXPECT_FALSE(Comparison::equal(-RML::INF, RML::INF));
	EXPECT_FALSE(Comparison::equal(RML::INF, -RML::INF));
	EXPECT_FALSE(Comparison::equal(RML::INF, 245123.123));
}
