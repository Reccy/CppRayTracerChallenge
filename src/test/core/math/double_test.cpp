#include "gtest/gtest.h"
#include "math/double.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Double, compare_exact_values)
{
	double doubleA = 1.0;
	double doubleB = 1.0;
	EXPECT_TRUE(Double::compare(doubleA, doubleB));
}

TEST(CppRayTracerChallenge_Core_Math_Double, compare_values_inside_epsilon)
{
	double doubleA = 1.0;
	double doubleB = 1.000001;
	EXPECT_TRUE(Double::compare(doubleA, doubleB));
}

TEST(CppRayTracerChallenge_Core_Math_Double, compare_values_outside_epsilon)
{
	double doubleA = 1.0;
	double doubleB = 1.00001;
	EXPECT_FALSE(Double::compare(doubleA, doubleB));
}
