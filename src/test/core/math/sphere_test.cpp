#include <gtest/gtest.h>
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Sphere, creating_a_sphere)
{
	Sphere sphere = Sphere();

	EXPECT_EQ(sphere.position(), Point(0, 0, 0));
	EXPECT_EQ(sphere.transform(), Transform());
}

TEST(CppRayTracerChallenge_Core_Math_Sphere, transforming_a_sphere)
{
	Transform transform = Transform()
		.translate(2, 3, 4);

	Sphere sphere = Sphere();
	sphere.transform(transform);

	EXPECT_EQ(sphere.transform(), transform);
}
