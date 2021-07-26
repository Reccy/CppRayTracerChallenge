#include <gtest/gtest.h>
#include <gtest/gtest-param-test.h>
#include "helpers/material_helper.h"
#include "renderer/computed_values.h"
#include "renderer/shape.h"
#include "renderer/material.h"
#include "math/intersection.h"
#include "math/sphere.h"
#include "math/plane.h"
#include "math/smooth_triangle.h"

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

class N1N2
{
public:
	int idx;
	float n1;
	float n2;
	
	N1N2(int _idx, float _n1, float _n2) : idx(_idx), n1(_n1), n2(_n2) {};

	friend std::ostream& operator<<(std::ostream& os, const N1N2& n1n2)
	{
		os << n1n2.idx << ", " << n1n2.n1 << ", " << n1n2.n2;
		return os;
	};
};

class CppRayTracerChallenge_Core_Renderer_ComputedValues_Params : public ::testing::TestWithParam<N1N2> {};

TEST_P(CppRayTracerChallenge_Core_Renderer_ComputedValues_Params, computes_n1_and_n2)
{
	auto a = Helpers::MaterialHelper::glassSphere();
	a.transform(Math::Transform().scale(2, 2, 2));
	auto aMat = a.material();
	aMat.refractiveIndex = 1.5;
	a.material(aMat);

	auto b = Helpers::MaterialHelper::glassSphere();
	a.transform(Math::Transform().translate(0, 0, -0.25f));
	auto bMat = b.material();
	bMat.refractiveIndex = 2.0f;
	b.material(bMat);

	auto c = Helpers::MaterialHelper::glassSphere();
	c.transform(Math::Transform().translate(0, 0, -0.25f));
	auto cMat = c.material();
	cMat.refractiveIndex = 2.5f;
	c.material(cMat);

	auto ray = Math::Ray({ 0, 0, -4 }, { 0, 0, 1 });
	std::vector<Math::Intersection> intersectionVector = {
		{ 2.0f, a },
		{ 2.75f, b },
		{ 3.25f, c },
		{ 4.75f, b },
		{ 5.25f, c },
		{ 6.0f, a }
	};
	auto intersections = Math::Intersections(intersectionVector);

	auto& param = GetParam();

	auto cv = Renderer::ComputedValues(intersections.at(param.idx), ray, intersections);

	EXPECT_EQ(cv.n1(), param.n1);
	EXPECT_EQ(cv.n2(), param.n2);
}

INSTANTIATE_TEST_SUITE_P(TestCase, CppRayTracerChallenge_Core_Renderer_ComputedValues_Params, ::testing::Values(
	N1N2(0, 1.0f, 1.5f),
	N1N2(1, 1.5f, 2.0f),
	N1N2(2, 2.0f, 2.5f),
	N1N2(3, 2.5f, 2.5f),
	N1N2(4, 2.5f, 1.5f),
	N1N2(5, 1.5f, 1.0f)
));

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, under_point_is_offset_below_the_surface)
{
	auto ray = Math::Ray({ 0, 0, -5 }, { 0, 0, 1 });
	auto shape = Helpers::MaterialHelper::glassSphere();
	shape.transform(Math::Transform().translate(0, 0, 1));
	auto intersection = Math::Intersection(5, shape);
	auto intersections = Math::Intersections({ intersection });

	auto cv = Renderer::ComputedValues(intersection, ray, intersections);

	EXPECT_TRUE(cv.underPosition().z() > Math::EPSILON / 2);
	EXPECT_TRUE(cv.position().z() < cv.underPosition().z());
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, reflectance_under_total_internal_reflection)
{
	auto shape = Helpers::MaterialHelper::glassSphere();
	auto ray = Math::Ray({ 0, 0, sqrt(2) / 2 }, { 0, 1, 0 });
	auto intersections = Math::Intersections({ { -sqrt(2) / 2, shape }, {sqrt(2) / 2, shape} });

	auto cv = Renderer::ComputedValues(intersections.at(1), ray, intersections);

	EXPECT_EQ(cv.reflectance(), 1.0f);
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, reflectance_with_perpendicular_viewing_angle)
{
	auto shape = Helpers::MaterialHelper::glassSphere();
	auto ray = Math::Ray({ 0,0,0 }, { 0,1,0 });
	auto intersections = Math::Intersections({ {-1, shape}, {1, shape} });

	auto cv = Renderer::ComputedValues(intersections.at(1), ray, intersections);

	float result = cv.reflectance();
	float expectedResult = 0.04f;

	EXPECT_TRUE(Math::Comparison::equal(result, expectedResult));
}

TEST(CppRayTracerChallenge_Core_Renderer_ComputedValues, reflectance_with_small_angle_and_n1_greater_than_n2)
{
	auto shape = Helpers::MaterialHelper::glassSphere();
	auto ray = Math::Ray({ 0, 0.99f, -2 }, { 0, 0, 1 });
	auto intersections = Math::Intersections({ { 1.8589, shape } });

	auto cv = Renderer::ComputedValues(intersections.at(0), ray, intersections);

	auto result = cv.reflectance();
	auto expectedResult = 0.488731f;

	EXPECT_TRUE(Math::Comparison::equal(result, expectedResult));
}

TEST(CppRayTracerChallenge_Core_Math_SmoothTriangle, prepares_normal_on_smooth_triangle)
{
	auto tri = std::make_shared<Math::SmoothTriangle>(Math::SmoothTriangle({ 0,1,0 }, { -1,0,0 }, { 1,0,0 }, { 0,1,0 }, { -1,0,0 }, { 1,0,0 }));
	auto shape = Renderer::Shape(tri);

	auto intersections = Math::Intersections({ { 1, shape, 0.45, 0.25 } });
	auto ray = Math::Ray({ -0.2, 0.3, -2 }, { 0, 0, 1 });

	auto cv = Renderer::ComputedValues(intersections.at(0), ray, intersections);

	auto expectedResult = Math::Vector(-0.5547, 0.83205, 0);

	EXPECT_EQ(cv.normal(), expectedResult);
}
