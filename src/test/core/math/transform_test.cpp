#include "gtest/gtest.h"
#include "math/transform.h"
#include "math/point.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Transform, translate_point)
{
	Transform transform = Transform()
		.translate(5, -3, 2);
	Point point = Point(-3, 4, 5);

	Point expectedResult = Point(2, 1, 7);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, inverse_point)
{
	Transform transform = Transform()
		.translate(5, -3, 2)
		.invert();
	Point point = Point(-3, 4, 5);

	Point expectedResult = Point(-8, 7, 3);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, translate_vector)
{
	Transform transform = Transform()
		.translate(5, -3, 2);
	Vector vector = Vector(-3, 4, 5);

	EXPECT_EQ(transform * vector, vector);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, scale_point)
{
	Transform transform = Transform()
		.scale(2, 3, 4);
	Point point = Point(-4, 6, 8);

	Point expectedResult = Point(-8, 18, 32);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, scale_vector)
{
	Transform transform = Transform()
		.scale(2, 3, 4);
	Vector vector = Vector(-4, 6, 8);

	Vector expectedResult = Vector(-8, 18, 32);

	EXPECT_EQ(transform * vector, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, scale_inverse)
{
	Transform transform = Transform()
		.scale(2, 3, 4)
		.invert();

	Vector vector = Vector(-4, 6, 8);

	Vector expectedResult = Vector(-2, 2, 2);

	EXPECT_EQ(transform * vector, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, scale_is_reflection)
{
	Transform transform = Transform()
		.scale(-1, 1, 1);

	Point point = Point(2, 3, 4);

	Point expectedResult = Point(-2, 3, 4);

	EXPECT_EQ(transform * point, expectedResult);
}
