#include <gtest/gtest.h>
#include "math/shape.h"

using namespace CppRayTracerChallenge::Core::Math;

/// <summary>
/// Shape only to be used in the test suite
/// </summary>
class TestingShape : public Shape
{
public:
	TestingShape() : Shape() {}

	const Vector normal(const Point position) const
	{
		return Vector(position);
	}
};

TEST(CppRayTracerChallenge_Core_Math_Shape, transforming_a_shape)
{
	Transform transform = Transform()
		.translate(2, 3, 4);

	TestingShape concreteShape = TestingShape();
	Shape& shape = concreteShape;
	shape.transform(transform);

	EXPECT_EQ(shape.transform(), transform);
}
