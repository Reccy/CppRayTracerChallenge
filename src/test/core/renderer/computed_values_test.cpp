#include <gtest/gtest.h>
#include "renderer/computed_values.h"
#include "renderer/sphere.h"
#include "renderer/material.h"
#include "math/intersection.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, hit_from_outside)
{
	Math::Ray ray = Math::Ray({ 0,0,-5 }, { 0,0,1 });
	Renderer::Material material = Renderer::Material();
	material.ambient = 200.0f;

	Renderer::Sphere sphere = Renderer::Sphere(material);
	Math::Intersection intersection = Math::Intersection(4, sphere);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_FALSE(cv.isInside());

	EXPECT_EQ(cv.shape().material(), material);
	EXPECT_EQ(cv.t(), 4);
	EXPECT_EQ(&cv.shape(), &sphere);
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, hit_from_inside)
{
	Math::Ray ray = Math::Ray({ 0,0,0 }, { 0,0,1 });
	Math::Sphere sphere = Math::Sphere();
	Math::Intersection intersection = Math::Intersection(1, sphere);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_TRUE(cv.isInside());

	EXPECT_EQ(cv.t(), 1);
	EXPECT_EQ(&cv.shape(), &sphere);
	EXPECT_EQ(cv.position(), Math::Point(0, 0, 1));
	EXPECT_EQ(cv.eye(), Math::Vector(0, 0, -1));
	EXPECT_EQ(cv.normal(), Math::Vector(0, 0, -1));
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, hit_offsets_point)
{
	Math::Ray ray = Math::Ray({ 0, 0, -5 }, { 0, 0, 1 });
	Math::Sphere sphere = Math::Sphere();
	sphere.transform(Math::Transform().translate(0, 0, 1));
	Math::Intersection intersection = Math::Intersection(5, sphere);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_TRUE(cv.overPosition().z() < -Math::EPSILON / 2);
	EXPECT_TRUE(cv.position().z() > cv.overPosition().z());
}
