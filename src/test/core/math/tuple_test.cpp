#include <gtest/gtest.h>
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Tuple, tuple_with_w1_is_a_point)
{
	Tuple tuple(4.3, -4.2, 3.1, 1.0);
	EXPECT_EQ(tuple.getX(), 4.3);
	EXPECT_EQ(tuple.getY(), -4.2);
	EXPECT_EQ(tuple.getZ(), 3.1);
	EXPECT_EQ(tuple.getW(), 1.0);
	EXPECT_EQ(tuple.isPoint(), true);
	EXPECT_EQ(tuple.isVector(), false);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, tuple_with_w0_is_a_vector)
{
	Tuple tuple(4.3, -4.2, 3.1, 0.0);
	EXPECT_EQ(tuple.getX(), 4.3);
	EXPECT_EQ(tuple.getY(), -4.2);
	EXPECT_EQ(tuple.getZ(), 3.1);
	EXPECT_EQ(tuple.getW(), 0.0);
	EXPECT_EQ(tuple.isPoint(), false);
	EXPECT_EQ(tuple.isVector(), true);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, buildPoint_will_build_tuple_with_w1)
{
	Tuple point = Tuple::buildPoint(4.3, -4.2, 3.1);
	Tuple expectedResult(4.3, -4.2, 3.1, 1.0);
	EXPECT_EQ(point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, buildVector_will_build_tuple_with_w0)
{
	Tuple vector = Tuple::buildVector(4.3, -4.2, 3.1);
	Tuple expectedResult(4.3, -4.2, 3.1, 0.0);
	EXPECT_EQ(vector, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, zeroVector_will_build_zero_vector)
{
	Tuple vector = Tuple::zeroVector();
	Tuple expectedResult(0, 0, 0, 0);
	EXPECT_EQ(vector, expectedResult);
}

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

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtracting_two_points_is_a_vector)
{
	Tuple pointA = Tuple::buildPoint(3, 2, 1);
	Tuple pointB = Tuple::buildPoint(5, 6, 7);
	Tuple expectedResult = Tuple::buildVector(-2, -4, -6);

	EXPECT_EQ((pointA - pointB), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtracting_a_vector_from_a_point_is_a_point)
{
	Tuple point = Tuple::buildPoint(3, 2, 1);
	Tuple vector = Tuple::buildVector(5, 6, 7);
	Tuple expectedResult = Tuple::buildPoint(-2, -4, -6);

	EXPECT_EQ((point - vector), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtracting_two_vectors_is_a_vector)
{
	Tuple vectorA = Tuple::buildVector(3, 2, 1);
	Tuple vectorB = Tuple::buildVector(5, 6, 7);
	Tuple expectedResult = Tuple::buildVector(-2, -4, -6);

	EXPECT_EQ((vectorA - vectorB), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, subtracting_a_vector_from_zero_vector_results_in_negation)
{
	Tuple zero = Tuple::zeroVector();
	Tuple vector = Tuple::buildVector(1, -2, 3);
	Tuple expectedResult = Tuple::buildVector(-1, 2, -3);

	EXPECT_EQ((zero - vector), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, unary_negation_will_negate_tuple)
{
	Tuple tuple = Tuple(1, -2, 3, -4);
	Tuple expectedResult = Tuple(-1, 2, -3, 4);

	EXPECT_EQ(-tuple, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_scalar)
{
	Tuple tuple = Tuple(1, -2, 3, -4);
	Tuple expectedResult = Tuple(3.5, -7, 10.5, -14);

	EXPECT_EQ(tuple * 3.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, multiplication_by_fraction)
{
	Tuple tuple = Tuple(1, -2, 3, -4);
	Tuple expectedResult = Tuple(0.5, -1, 1.5, -2);

	EXPECT_EQ(tuple * 0.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, dividing_by_scalar)
{
	Tuple tuple = Tuple(1, -2, 3, -4);
	Tuple expectedResult = Tuple(0.5, -1, 1.5, -2);

	EXPECT_EQ(tuple / 2, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, magnitude_of_vector_1_0_0)
{
	Tuple vector = Tuple::buildVector(1, 0, 0);
	double expectedResult = 1;

	EXPECT_EQ(vector.magnitude(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, magnitude_of_vector_0_1_0)
{
	Tuple vector = Tuple::buildVector(0, 1, 0);
	double expectedResult = 1;

	EXPECT_EQ(vector.magnitude(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, magnitude_of_vector_0_0_1)
{
	Tuple vector = Tuple::buildVector(0, 0, 1);
	double expectedResult = 1;

	EXPECT_EQ(vector.magnitude(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, magnitude_of_vector_1_2_3)
{
	Tuple vector = Tuple::buildVector(1, 2, 3);
	double expectedResult = 3.7416573867739413; // sqrt(14)

	EXPECT_EQ(vector.magnitude(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, magnitude_of_vector_neg1_neg2_neg3)
{
	Tuple vector = Tuple::buildVector(-1, -2, -3);
	double expectedResult = 3.7416573867739413; // sqrt(14)

	EXPECT_EQ(vector.magnitude(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, normalization_of_vector_4_0_0)
{
	Tuple vector = Tuple::buildVector(4, 0, 0);
	Tuple expectedResult = Tuple::buildVector(1, 0, 0);

	EXPECT_EQ(vector.normalize(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, normalization_of_vector_1_2_3)
{
	Tuple vector = Tuple::buildVector(1, 2, 3);
	Tuple expectedResult = Tuple::buildVector(0.267261f, 0.534522f, 0.801784f);

	EXPECT_EQ(vector.normalize(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, magnitude_of_normalized_vector)
{
	Tuple vector = Tuple::buildVector(1, 2, 3);
	double expectedResult = 1.0;

	vector = vector.normalize();

	EXPECT_EQ(vector.magnitude(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, ostream_will_show_x_y_z_w_components)
{
	Tuple tuple(4.3, -4.2, 3.1, 1.0);
	std::stringstream ss;
	ss << tuple;

	EXPECT_EQ(ss.str(), "4.3, -4.2, 3.1, 1");
}
