#include <math.h>

#include "gtest/gtest.h"
#include "math/transform.h"
#include "math/point.h"
#include "math/trig.h"

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

TEST(CppRayTracerChallenge_Core_Math_Transform, transpose)
{
	Transform transform = Transform()
		.shear(1,0,0,1,2,3)
		.transpose();

	Point point = Point(0, 1, 0);

	Point expectedResult = Point(0, 1, 1);

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

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_around_x_full_quarter)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 2);

	Transform transform = Transform()
		.rotate(rotationAmount, 0, 0);

	Point point = Point(0, 1, 0);

	Point expectedResult = Point(0, 0, 1);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_around_x_half_quarter)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 4);

	Transform transform = Transform()
		.rotate(rotationAmount, 0, 0);

	Point point = Point(0, 1, 0);

	Point expectedResult = Point(0, sqrt(2) / 2, (sqrt(2) / 2));

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_x_inverse)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 4);

	Transform transform = Transform()
		.rotate(rotationAmount, 0, 0)
		.invert();

	Point point = Point(0, 1, 0);

	Point expectedResult = Point(0, sqrt(2) / 2, -(sqrt(2) / 2));

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_around_y_half_quarter)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 4);

	Transform transform = Transform()
		.rotate(0, rotationAmount, 0);

	Point point = Point(0, 0, 1);

	Point expectedResult = Point(sqrt(2) / 2, 0, (sqrt(2) / 2));

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_around_y_full_quarter)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 2);

	Transform transform = Transform()
		.rotate(0, rotationAmount, 0);

	Point point = Point(0, 0, 1);

	Point expectedResult = Point(1, 0, 0);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_y_inverse)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 4);

	Transform transform = Transform()
		.rotate(0, rotationAmount, 0)
		.invert();

	Point point = Point(0, 0, 1);

	Point expectedResult = Point(-(sqrt(2) / 2), 0, sqrt(2) / 2);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_around_z_half_quarter)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 4);

	Transform transform = Transform()
		.rotate(0, 0, rotationAmount);

	Point point = Point(0, 1, 0);

	Point expectedResult = Point(-(sqrt(2) / 2), sqrt(2) / 2, 0);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_around_z_full_quarter)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 2);

	Transform transform = Transform()
		.rotate(0, 0, rotationAmount);

	Point point = Point(0, 1, 0);

	Point expectedResult = Point(-1, 0, 0);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_z_inverse)
{
	double rotationAmount = Trig::radians_to_degrees(Trig::PI / 4);

	Transform transform = Transform()
		.rotate(0, 0, rotationAmount);

	Point point = Point(0, 1, 0);

	Point expectedResult = Point(-(sqrt(2) / 2), sqrt(2) / 2, 0);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_all_axes_is_applied_correctly)
{
	Transform transformA = Transform()
		.rotate(45, 90, 270);

	Transform transformB = Transform()
		.rotate(45, 0, 0)
		.rotate(0, 90, 0)
		.rotate(0, 0, 270);

	Point point = Point(0, 1, 0);

	EXPECT_EQ(transformA * point, transformB * point);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, shear_moves_x_in_proportion_to_y)
{
	Transform transform = Transform()
		.shear(1, 0, 0, 0, 0, 0);

	Point point = Point(2, 3, 4);

	Point expectedResult = Point(5, 3, 4);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, shear_moves_x_in_proportion_to_z)
{
	Transform transform = Transform()
		.shear(0, 1, 0, 0, 0, 0);

	Point point = Point(2, 3, 4);

	Point expectedResult = Point(6, 3, 4);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, shear_moves_y_in_proportion_to_x)
{
	Transform transform = Transform()
		.shear(0, 0, 1, 0, 0, 0);

	Point point = Point(2, 3, 4);

	Point expectedResult = Point(2, 5, 4);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, shear_moves_y_in_proportion_to_z)
{
	Transform transform = Transform()
		.shear(0, 0, 0, 1, 0, 0);

	Point point = Point(2, 3, 4);

	Point expectedResult = Point(2, 7, 4);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, shear_moves_z_in_proportion_to_x)
{
	Transform transform = Transform()
		.shear(0, 0, 0, 0, 1, 0);

	Point point = Point(2, 3, 4);

	Point expectedResult = Point(2, 3, 6);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, shear_moves_z_in_proportion_to_y)
{
	Transform transform = Transform()
		.shear(0, 0, 0, 0, 0, 1);

	Point point = Point(2, 3, 4);

	Point expectedResult = Point(2, 3, 7);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, individual_transformations_are_applied_in_sequence)
{
	Point point = Point(1, 0, 1);

	Transform a = Transform()
		.rotate(Trig::radians_to_degrees(Trig::PI / 2), 0, 0);

	Transform b = Transform()
		.scale(5, 5, 5);

	Transform c = Transform()
		.translate(10, 5, 7);

	point = a * point;
	EXPECT_EQ(point, Point(1, -1, 0));

	point = b * point;
	EXPECT_EQ(point, Point(5, -5, 0));

	point = c * point;
	EXPECT_EQ(point, Point(15, 0, 7));
}

TEST(CppRayTracerChallenge_Core_Math_Transform, chained_transformations_are_applied_in_reverse_order)
{
	Point point = Point(1, 0, 1);

	Transform transform = Transform()
		.rotate(Trig::radians_to_degrees(Trig::PI / 2), 0, 0)
		.scale(5, 5, 5)
		.translate(10, 5, 7);

	Point expectedResult = Point(15, 0, 7);

	EXPECT_EQ(transform * point, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, translate_does_not_mutate_original)
{
	Transform original = Transform();

	Transform mutated = original.translate(1, 2, 3);

	EXPECT_TRUE(original != mutated);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, scale_does_not_mutate_original)
{
	Transform original = Transform();

	Transform mutated = original.scale(1, 2, 3);

	EXPECT_TRUE(original != mutated);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, rotate_does_not_mutate_original)
{
	Transform original = Transform();

	Transform mutated = original.rotate(1, 2, 3);

	EXPECT_TRUE(original != mutated);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, shear_does_not_mutate_original)
{
	Transform original = Transform();

	Transform mutated = original.shear(1, 2, 3, 1, 2, 3);

	EXPECT_TRUE(original != mutated);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, invert_does_not_mutate_original)
{
	Transform original = Transform()
		.translate(1,0,0);

	Transform mutated = original.invert();

	EXPECT_TRUE(original != mutated);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, read_matrix)
{
	Transform original = Transform()
		.translate(1, 0, 0);

	auto expectedResult = Matrix<double, 4, 4>({
		1,0,0,1,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	});

	EXPECT_EQ(original.matrix(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, equality)
{
	Transform a = Transform()
		.scale(1, 2, 3)
		.rotate(1, 2, 3)
		.invert()
		.shear(1, 2, 3, 1, 2, 3);

	Transform b = Transform()
		.scale(1, 2, 3)
		.rotate(1, 2, 3)
		.invert()
		.shear(1, 2, 3, 1, 2, 3);

	EXPECT_TRUE(a == b);
}

TEST(CppRayTracerChallenge_Core_Math_Transform, inequality)
{
	Transform a = Transform()
		.scale(1, 2, 3)
		.rotate(1, 2, 3)
		.invert()
		.shear(1, 2, 3, 1, 2, 3);

	Transform b = Transform()
		.scale(1, 2, 3)
		.rotate(1, 2, 3)
		.shear(1, 2, 3, 1, 2, 3)
		.invert();

	EXPECT_TRUE(a != b);
}
