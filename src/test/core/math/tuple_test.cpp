#include <gtest/gtest.h>
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Tuple, tuple_with_w1_is_a_point)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_EQ(tuple.getX(), 4.3f);
	EXPECT_EQ(tuple.getY(), -4.2f);
	EXPECT_EQ(tuple.getZ(), 3.1f);
	EXPECT_EQ(tuple.getW(), 1.0);
	EXPECT_EQ(tuple.isPoint(), true);
	EXPECT_EQ(tuple.isVector(), false);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, tuple_with_w0_is_a_vector)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 0.0f);
	EXPECT_EQ(tuple.getX(), 4.3f);
	EXPECT_EQ(tuple.getY(), -4.2f);
	EXPECT_EQ(tuple.getZ(), 3.1f);
	EXPECT_EQ(tuple.getW(), 0.0f);
	EXPECT_EQ(tuple.isPoint(), false);
	EXPECT_EQ(tuple.isVector(), true);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, buildPoint_will_build_tuple_with_w1)
{
	Tuple point = Tuple::buildPoint(4.3f, -4.2f, 3.1f);
	Tuple tuple(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_EQ(point, tuple);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, buildVector_will_build_tuple_with_w0)
{
	Tuple vector = Tuple::buildVector(4.3f, -4.2f, 3.1f);
	Tuple tuple(4.3f, -4.2f, 3.1f, 0.0f);
	EXPECT_EQ(vector, tuple);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, zeroVector_will_build_zero_vector)
{
	Tuple vector = Tuple::zeroVector();
	Tuple tuple(0, 0, 0, 0);
	EXPECT_EQ(vector, tuple);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_true)
{
	Tuple tupleA(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple tupleB(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_TRUE(tupleA == tupleB);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_false)
{
	Tuple tupleA(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple tupleB(1.0f, 2.0f, -9.3f, 0.0f);
	EXPECT_FALSE(tupleA == tupleB);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_true)
{
	Tuple tupleA(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple tupleB(1.0f, 2.0f, -9.3f, 0.0f);
	EXPECT_TRUE(tupleA != tupleB);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_false)
{
	Tuple tupleA(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple tupleB(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_FALSE(tupleA != tupleB);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, addition)
{
	Tuple tupleA(3, -2, 5, 1);
	Tuple tupleB(-2, 3, 1, 0);
	Tuple expectedResult(1, 1, 6, 1);

	EXPECT_TRUE((tupleA + tupleB) == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtracting_two_points_is_a_vector)
{
	Tuple pointA = Tuple::buildPoint(3, 2, 1);
	Tuple pointB = Tuple::buildPoint(5, 6, 7);
	Tuple expectedResult = Tuple::buildVector(-2, -4, -6);

	EXPECT_TRUE((pointA - pointB) == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtracting_a_vector_from_a_point_is_a_point)
{
	Tuple point = Tuple::buildPoint(3, 2, 1);
	Tuple vector = Tuple::buildVector(5, 6, 7);
	Tuple expectedResult = Tuple::buildPoint(-2, -4, -6);

	EXPECT_TRUE((point - vector) == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtracting_two_vectors_is_a_vector)
{
	Tuple vectorA = Tuple::buildVector(3, 2, 1);
	Tuple vectorB = Tuple::buildVector(5, 6, 7);
	Tuple expectedResult = Tuple::buildVector(-2, -4, -6);

	EXPECT_TRUE((vectorA - vectorB) == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtracting_a_vector_from_zero_vector_results_in_negation)
{
	Tuple zero = Tuple::zeroVector();
	Tuple vector = Tuple::buildVector(1, -2, 3);
	Tuple expectedResult = Tuple::buildVector(-1, 2, -3);

	EXPECT_TRUE((zero - vector) == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, unary_negation_will_negate_tuple)
{
	Tuple vector = Tuple(1, -2, 3, -4);
	Tuple expectedResult = Tuple(-1, 2, -3, 4);

	EXPECT_TRUE(-vector == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_scalar)
{
	Tuple vector = Tuple(1, -2, 3, -4);
	Tuple expectedResult = Tuple(3.5, -7, 10.5, -14);

	EXPECT_EQ(vector * 3.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_fraction)
{
	Tuple vector = Tuple(1, -2, 3, -4);
	Tuple expectedResult = Tuple(0.5, -1, 1.5, -2);

	EXPECT_EQ(vector * 0.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, ostream_will_show_x_y_z_w_components)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 1.0f);
	std::stringstream ss;
	ss << tuple;

	EXPECT_EQ(ss.str(), "4.3, -4.2, 3.1, 1");
}
