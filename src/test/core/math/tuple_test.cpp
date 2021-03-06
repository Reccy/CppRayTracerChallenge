#include <gtest/gtest.h>
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Tuple, x)
{
	Tuple<double> tuple(1.0, 2.0, 3.0, 4.0);

	double expectedResult = 1.0;

	EXPECT_EQ(tuple.x(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, y)
{
	Tuple<double> tuple(1.0, 2.0, 3.0, 4.0);

	double expectedResult = 2.0;

	EXPECT_EQ(tuple.y(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, z)
{
	Tuple tuple(1.0, 2.0, 3.0, 4.0);

	double expectedResult = 3.0;

	EXPECT_EQ(tuple.z(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, w)
{
	Tuple tuple(1.0, 2.0, 3.0, 4.0);

	double expectedResult = 4.0;

	EXPECT_EQ(tuple.w(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_true_double)
{
	Tuple<double> tuple(4.3, -4.2, 3.1, 1.0);
	Tuple<double> expectedResult(4.3, -4.2, 3.1, 1.0);
	EXPECT_TRUE(tuple == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_false_double)
{
	Tuple<double> tuple(4.3, -4.2, 3.1, 1.0);
	Tuple<double> expectedResult(1.0, 2.0, -9.3, 0.0);
	EXPECT_FALSE(tuple == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_true_double)
{
	Tuple<double> tuple(4.3, -4.2, 3.1, 1.0);
	Tuple<double> expectedResult(1.0, 2.0, -9.3, 0.0);
	EXPECT_TRUE(tuple != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_false_double)
{
	Tuple<double> tuple(4.3, -4.2, 3.1, 1.0);
	Tuple<double> expectedResult(4.3, -4.2, 3.1, 1.0);
	EXPECT_FALSE(tuple != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_true_float)
{
	Tuple<float> tuple(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple<float> expectedResult(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_TRUE(tuple == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_false_float)
{
	Tuple<float> tuple(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple<float> expectedResult(1.0f, 2.0f, -9.3f, 0.0f);
	EXPECT_FALSE(tuple == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_true_float)
{
	Tuple<float> tuple(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple<float> expectedResult(1.0f, 2.0f, -9.3f, 0.0f);
	EXPECT_TRUE(tuple != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_false_float)
{
	Tuple<float> tuple(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple<float> expectedResult(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_FALSE(tuple != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_true_int)
{
	Tuple<int> tuple(4, -4, 3, 1);
	Tuple<int> expectedResult(4, -4, 3, 1);
	EXPECT_TRUE(tuple == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_false_int)
{
	Tuple<int> tuple(4, -4, 3, 1);
	Tuple<int> expectedResult(1, 2, -9, 0);
	EXPECT_FALSE(tuple == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_true_int)
{
	Tuple<int> tuple(4, -4, 3, 1);
	Tuple<int> expectedResult(1, 2, -9, 0);
	EXPECT_TRUE(tuple != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_false_int)
{
	Tuple<int> tuple(4, -4, 3, 1);
	Tuple<int> expectedResult(4, -4, 3, 1);
	EXPECT_FALSE(tuple != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, addition)
{
	Tuple<double> tupleA(3, -2, 5, 1);
	Tuple<double> tupleB(-2, 3, 1, 0);
	Tuple<double> expectedResult(1, 1, 6, 1);

	EXPECT_EQ((tupleA + tupleB), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, addition_compound)
{
	Tuple<double> tupleA(3, -2, 5, 1);
	Tuple<double> tupleB(-2, 3, 1, 0);
	Tuple<double> expectedResult(1, 1, 6, 1);

	tupleA += tupleB;

	EXPECT_EQ(tupleA, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtraction)
{
	Tuple<double> tupleA(3, -2, 5, 1);
	Tuple<double> tupleB(-2, 3, 1, 0);
	Tuple<double> expectedResult(5, -5, 4, 1);

	EXPECT_EQ((tupleA - tupleB), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtraction_compound)
{
	Tuple<double> tupleA(3, -2, 5, 1);
	Tuple<double> tupleB(-2, 3, 1, 0);
	Tuple<double> expectedResult(5, -5, 4, 1);

	tupleA -= tupleB;

	EXPECT_EQ(tupleA, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, unary_negation_will_negate_tuple)
{
	Tuple<double> tuple(1, -2, 3, -4);
	Tuple<double> expectedResult(-1, 2, -3, 4);

	EXPECT_EQ(-tuple, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_scalar)
{
	Tuple<double> tuple(1, -2, 3, -4);
	Tuple<double> expectedResult(3.5, -7, 10.5, -14);

	EXPECT_EQ(tuple * 3.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_scalar_compound)
{
	Tuple<double> tuple(1, -2, 3, -4);
	Tuple<double> expectedResult(3.5, -7, 10.5, -14);

	tuple *= 3.5;

	EXPECT_EQ(tuple, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_fraction)
{
	Tuple<double> tuple(1, -2, 3, -4);
	Tuple<double> expectedResult(0.5, -1, 1.5, -2);

	EXPECT_EQ(tuple * 0.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_fraction_compound)
{
	Tuple<double> tuple(1, -2, 3, -4);
	Tuple<double> expectedResult(0.5, -1, 1.5, -2);

	tuple *= 0.5;

	EXPECT_EQ(tuple, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, dividing_by_scalar)
{
	Tuple<double> tuple(1, -2, 3, -4);
	Tuple<double> expectedResult(0.5, -1, 1.5, -2);

	EXPECT_EQ(tuple / 2, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, dividing_by_scalar_compound)
{
	Tuple<double> tuple(1, -2, 3, -4);
	Tuple<double> expectedResult(0.5, -1, 1.5, -2);

	tuple /= 2;

	EXPECT_EQ(tuple, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, ostream_will_show_x_y_z_w_components)
{
	Tuple<double> tuple(4.3, -4.2, 3.1, 1.0);
	std::stringstream ss;
	ss << tuple;

	EXPECT_EQ(ss.str(), "4.3, -4.2, 3.1, 1");
}
