#include <gtest/gtest.h>
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_true)
{
	Tuple tuple(4.3, -4.2, 3.1, 1.0);
	Tuple expectedResult(4.3, -4.2, 3.1, 1.0);
	EXPECT_TRUE(tuple == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_false)
{
	Tuple tuple(4.3, -4.2, 3.1, 1.0);
	Tuple expectedResult(1.0, 2.0, -9.3, 0.0);
	EXPECT_FALSE(tuple == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_true)
{
	Tuple tuple(4.3, -4.2, 3.1, 1.0);
	Tuple expectedResult(1.0, 2.0, -9.3, 0.0);
	EXPECT_TRUE(tuple != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_false)
{
	Tuple tuple(4.3, -4.2, 3.1, 1.0);
	Tuple expectedResult(4.3, -4.2, 3.1, 1.0);
	EXPECT_FALSE(tuple != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, addition)
{
	Tuple tupleA(3, -2, 5, 1);
	Tuple tupleB(-2, 3, 1, 0);
	Tuple expectedResult(1, 1, 6, 1);

	EXPECT_EQ((tupleA + tupleB), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtraction)
{
	Tuple tupleA(3, -2, 5, 1);
	Tuple tupleB(-2, 3, 1, 0);
	Tuple expectedResult(5, -5, 4, 1);

	EXPECT_EQ((tupleA - tupleB), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, unary_negation_will_negate_tuple)
{
	Tuple tuple(1, -2, 3, -4);
	Tuple expectedResult(-1, 2, -3, 4);

	EXPECT_EQ(-tuple, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_scalar)
{
	Tuple tuple(1, -2, 3, -4);
	Tuple expectedResult(3.5, -7, 10.5, -14);

	EXPECT_EQ(tuple * 3.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_fraction)
{
	Tuple tuple(1, -2, 3, -4);
	Tuple expectedResult(0.5, -1, 1.5, -2);

	EXPECT_EQ(tuple * 0.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, dividing_by_scalar)
{
	Tuple tuple(1, -2, 3, -4);
	Tuple expectedResult(0.5, -1, 1.5, -2);

	EXPECT_EQ(tuple / 2, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, ostream_will_show_x_y_z_w_components)
{
	Tuple tuple(4.3, -4.2, 3.1, 1.0);
	std::stringstream ss;
	ss << tuple;

	EXPECT_EQ(ss.str(), "4.3, -4.2, 3.1, 1");
}
