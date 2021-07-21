#include <gtest/gtest.h>
#include <math/triangle.h>

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Triangle, constructing_a_triangle)
{
	Point p1 = Point(0, 1, 0);
	Point p2 = Point(-1, 0, 0);
	Point p3 = Point(1, 0, 0);
	Triangle triangle = Triangle(p1, p2, p3);

	EXPECT_EQ(triangle.p1(), p1);
	EXPECT_EQ(triangle.p2(), p2);
	EXPECT_EQ(triangle.p3(), p3);

	EXPECT_EQ(triangle.e1(), Vector(-1, -1, 0));
	EXPECT_EQ(triangle.e2(), Vector(1, -1, 0));

	EXPECT_EQ(triangle.normal(), Vector(0, 0, -1));
}
