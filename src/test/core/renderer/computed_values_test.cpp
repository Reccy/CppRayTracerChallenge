#include <gtest/gtest.h>
#include "renderer/computed_values.h"
#include "renderer/shape.h"
#include "renderer/material.h"
#include "math/intersection.h"
#include "math/sphere.h"
#include "math/plane.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, hit_from_outside)
{
	Math::Ray ray = Math::Ray({ 0,0,-5 }, { 0,0,1 });
	Renderer::Material material = Renderer::Material();
	material.ambient = 200.0f;

	auto sphere = std::make_shared<Math::Sphere>();
	Renderer::Shape shape = Renderer::Shape(sphere, material);
	Math::Intersection intersection = Math::Intersection(4, shape);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_FALSE(cv.isInside());

	EXPECT_EQ(cv.material(), material);
	EXPECT_EQ(cv.t(), 4);
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, hit_from_inside)
{
	Math::Ray ray = Math::Ray({ 0,0,0 }, { 0,0,1 });
	auto sphere = std::make_shared<Math::Sphere>();
	Renderer::Shape shape = Renderer::Shape(sphere, Renderer::Material());
	Math::Intersection intersection = Math::Intersection(1, shape);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_TRUE(cv.isInside());

	EXPECT_EQ(cv.t(), 1);
	EXPECT_EQ(cv.position(), Math::Point(0, 0, 1));
	EXPECT_EQ(cv.eye(), Math::Vector(0, 0, -1));
	EXPECT_EQ(cv.normal(), Math::Vector(0, 0, -1));
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, hit_offsets_point)
{
	Math::Ray ray = Math::Ray({ 0, 0, -5 }, { 0, 0, 1 });
	auto sphere = std::make_shared<Math::Sphere>();
	Renderer::Shape shape = Renderer::Shape(sphere, Renderer::Material());
	shape.transform(Math::Transform().translate(0, 0, 1));
	Math::Intersection intersection = Math::Intersection(5, shape);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_TRUE(cv.overPosition().z() < -Math::EPSILON / 2);
	EXPECT_TRUE(cv.position().z() > cv.overPosition().z());
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, computes_reflection_vector)
{
	auto plane = std::make_shared<Math::Plane>();
	Renderer::Shape shape = Renderer::Shape(plane, Renderer::Material());
	Math::Ray ray = Math::Ray({ 0, 1, -1 }, { 0, -sqrt(2) / 2, sqrt(2) / 2 });
	Math::Intersection intersection = Math::Intersection(sqrt(2), shape);

	Renderer::ComputedValues cv = Renderer::ComputedValues(intersection, ray);

	EXPECT_EQ(cv.reflect(), Math::Vector(0, sqrt(2) / 2, sqrt(2) / 2));
}
