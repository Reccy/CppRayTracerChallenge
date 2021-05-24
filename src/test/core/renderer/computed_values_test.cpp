#include <gtest/gtest.h>
#include "renderer/computed_values.h"
#include "math/intersection.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, hit_from_outside)
{
	Math::Ray ray = Math::Ray({ 0,0,-5 }, { 0,0,1 });
	Math::Sphere* sphere = new Math::Sphere();
	Math::Intersection intersection = Math::Intersection(4, *sphere);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_FALSE(cv.isInside());

	EXPECT_EQ(cv.t(), 4);
	EXPECT_EQ(&cv.intersectable(), sphere);
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, hit_from_inside)
{
	Math::Ray ray = Math::Ray({ 0,0,0 }, { 0,0,1 });
	Math::Sphere* sphere = new Math::Sphere();
	Math::Intersection intersection = Math::Intersection(1, *sphere);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_TRUE(cv.isInside());

	EXPECT_EQ(cv.t(), 1);
	EXPECT_EQ(&cv.intersectable(), sphere);
	EXPECT_EQ(cv.position(), Math::Point(0, 0, 1));
	EXPECT_EQ(cv.eye(), Math::Vector(0, 0, -1));
	EXPECT_EQ(cv.normal(), Math::Vector(0, 0, -1));
}
