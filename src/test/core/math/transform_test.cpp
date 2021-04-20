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
