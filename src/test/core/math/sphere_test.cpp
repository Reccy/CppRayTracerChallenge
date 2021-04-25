#include <gtest/gtest.h>
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Sphere, creating_a_sphere)
{
	Sphere sphere = Sphere();

	EXPECT_EQ(sphere.position(), Point(0, 0, 0));
	EXPECT_EQ(sphere.radius(), 1);
}
