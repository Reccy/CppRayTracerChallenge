#include "gtest/gtest.h"
#include "math/tuple.h"
#include "math/vector.h"
#include "math/point.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Point, construct_from_tuple)
{
	Tuple tuple(1, 2, 3, 4);
	Point point(tuple);

	Point expectedResult(1, 2, 3);

	EXPECT_EQ(point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, x)
{
	Point pointA(1, 2, 3);

	double expectedResult = 1;

	EXPECT_EQ(pointA.x(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, y)
{
	Point pointA(1, 2, 3);

	double expectedResult = 2;

	EXPECT_EQ(pointA.y(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, z)
{
	Point pointA(1, 2, 3);

	double expectedResult = 3;

	EXPECT_EQ(pointA.z(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, w_is_1)
{
	Point pointA(1, 2, 3);

	double expectedResult = 1;

	EXPECT_EQ(pointA.w(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, equality_true)
{
	Point point(4.3, -4.2, 3.1);
	Point expectedResult(4.3, -4.2, 3.1);
	EXPECT_TRUE(point == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, equality_false)
{
	Point point(4.3, -4.2, 3.1);
	Point expectedResult(1.0, 2.0, -9.3);
	EXPECT_FALSE(point == expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, inequality_true)
{
	Point point(4.3, -4.2, 3.1);
	Point expectedResult(1.0, 2.0, -9.3);
	EXPECT_TRUE(point != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, inequality_false)
{
	Point point(4.3, -4.2, 3.1);
	Point expectedResult(4.3, -4.2, 3.1);
	EXPECT_FALSE(point != expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, adding_two_points_returns_a_tuple)
{
	Point pointA(3, -2, 5);
	Point pointB(-2, 3, 1);
	Tuple expectedResult(1, 1, 6, 2);

	EXPECT_EQ((pointA + pointB), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, subtracting_two_points_is_a_vector)
{
	Point pointA(3, -2, 5);
	Point pointB(-2, 3, 1);
	Vector expectedResult(5, -5, 4);

	EXPECT_EQ((pointA - pointB), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, subtracting_vector_from_point_is_a_point)
{
	Point point(3, 2, 1);
	Vector vector(5, 6, 7);
	Point expectedResult(-2, -4, -6);

	EXPECT_EQ((point - vector), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, unary_negation_will_negate_and_transform_into_tuple)
{
	Point point(1, -2, 3);
	Tuple expectedResult(-1, 2, -3, -1);

	EXPECT_EQ(-point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, multiplication_by_scalar_and_transform_into_tuple)
{
	Point point(1, -2, 3);
	Tuple expectedResult(3.5, -7, 10.5, 3.5);

	EXPECT_EQ(point * 3.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, multiplication_by_fraction_and_transform_into_tuple)
{
	Point point(1, -2, 3);
	Tuple expectedResult(0.5, -1, 1.5, 0.5);

	EXPECT_EQ(point * 0.5, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, dividing_by_scalar_and_transform_into_tuple)
{
	Point point(1, -2, 3);
	Tuple expectedResult(0.5, -1, 1.5, 0.5);

	EXPECT_EQ(point / 2, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Point, ostream_will_show_x_y_z_w_components)
{
	Point tuple(4.3, -4.2, 3.1);
	std::stringstream ss;
	ss << tuple;

	EXPECT_EQ(ss.str(), "4.3, -4.2, 3.1, 1");
}
