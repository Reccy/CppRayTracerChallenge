#include <gtest/gtest.h>
#include <RML.h>
#include "renderer/world.h"
#include "renderer/computed_values.h"
#include "renderer/patterns/solid_color.h"
#include "renderer/patterns/test_pattern.h"
#include "math/ray.h"
#include "math/intersections.h"
#include "math/sphere.h"
#include "math/plane.h"
#include "helpers/material_helper.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	TEST(CppRayTracerChallenge_Core_Renderer_World, world_creation)
	{
		World world = World();
		EXPECT_EQ(world.objectCount(), 0);
		EXPECT_EQ(world.lightCount(), 0);
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, default_world)
	{
		World world = World::defaultWorld();

		Shape obj1 = world.objectAt(0);
		Shape obj2 = world.objectAt(1);
		const PointLight& light1 = world.lightAt(0);

		Material s1mat = Material();
		s1mat.pattern = std::make_shared<Renderer::Patterns::SolidColor>(Renderer::Patterns::SolidColor(Graphics::Color(0.8f, 1.0f, 0.6f)));
		s1mat.diffuse = 0.7f;
		s1mat.specular = 0.2f;

		EXPECT_EQ(obj1.material(), s1mat);

		Math::Transform s2transform = Math::Transform()
			.scale(0.5, 0.5, 0.5);

		EXPECT_EQ(obj2.transform(), s2transform);

		EXPECT_EQ(light1.position(), RML::Point(-10, 10, -10));
		EXPECT_EQ(light1.intensity(), Graphics::Color::white());

		EXPECT_EQ(world.lightCount(), 1);
		EXPECT_EQ(world.objectCount(), 2);
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, clear_lights)
	{
		World world = World::defaultWorld();
		EXPECT_EQ(world.lightCount(), 1);

		world.clearLights();

		EXPECT_EQ(world.lightCount(), 0);
		EXPECT_EQ(world.objectCount(), 2);
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, intersect_ray)
	{
		World world = World::defaultWorld();
		Math::Ray ray = Math::Ray({ 0, 0, -5 }, { 0,0,1 });

		Math::Intersections intersections = world.intersectRay(ray);

		EXPECT_EQ(intersections.count(), 4);
		EXPECT_EQ(intersections.at(0).t(), 4);
		EXPECT_EQ(intersections.at(1).t(), 4.5);
		EXPECT_EQ(intersections.at(2).t(), 5.5);
		EXPECT_EQ(intersections.at(3).t(), 6);
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, shade_hit)
	{
		World world = World::defaultWorld();
		Math::Ray ray = Math::Ray({ 0, 0, -5 }, { 0, 0, 1 });
		Math::Intersections intersections = world.intersectRay(ray);
		const Math::Intersection& intersection = intersections.hit().value();

		ComputedValues cv = ComputedValues(intersection, ray);

		Graphics::Color color = world.shadeHit(cv);

		EXPECT_EQ(color, Graphics::Color(0.38066f, 0.47583f, 0.2855f));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, shade_hit_from_inside)
	{
		World world = World::defaultWorld();
		Math::Ray ray = Math::Ray({ 0, 0, 0 }, { 0, 0, 1 });

		world.clearLights();
		world.addLight(Renderer::PointLight({ 0, 0.25, 0 }, Graphics::Color::white()));

		Math::Intersections intersections = world.intersectRay(ray);
		const Math::Intersection& intersection = intersections.hit().value();

		ComputedValues cv = ComputedValues(intersection, ray);

		Graphics::Color color = world.shadeHit(cv);

		EXPECT_EQ(color, Graphics::Color(0.90498f, 0.90498f, 0.90498f));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, shade_hit_given_shadowed_intersection)
	{
		World world = World::World();
		world.addLight(PointLight({ 0, 0, -10 }, Graphics::Color::white()));

		auto sphere1 = std::make_shared<Math::Sphere>(Math::Sphere());
		world.addObject(Renderer::Shape(sphere1));

		auto sphere2 = std::make_shared<Math::Sphere>(Math::Sphere());
		Renderer::Shape s = Renderer::Shape(sphere2);
		s.transform(Math::Transform().translate(0, 0, 10));
		world.addObject(s);

		Math::Ray ray = Math::Ray({ 0, 0, 5 }, { 0, 0, 1 });

		Graphics::Color result = world.colorAt(ray);

		EXPECT_EQ(result, Graphics::Color(0.1f, 0.1f, 0.1f));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, shade_hit_for_reflected_material)
	{
		World world = World::defaultWorld();
		auto plane = std::make_shared<Math::Plane>();
		Renderer::Material mat = Renderer::Material();
		mat.reflective = 0.5f;
		Renderer::Shape shape = Renderer::Shape(plane, mat);
		shape.transform(Math::Transform().translate(0, -1, 0));

		world.addObject(shape);
		Math::Ray ray = Math::Ray({ 0, 0, -3 }, { 0, -sqrt(2) / 2, sqrt(2) / 2 });
		Math::Intersection intersection = Math::Intersection(sqrt(2), shape);

		ComputedValues cv = ComputedValues(intersection, ray);
		Graphics::Color result = world.shadeHit(cv);

		Graphics::Color expectedResult = Graphics::Color(0.876758f, 0.924341f, 0.829175f);

		EXPECT_EQ(result, expectedResult);
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, shade_hit_for_refraction)
	{
		World world = World::defaultWorld();

		Renderer::Material floorMat;
		floorMat.transparency = 0.5f;
		floorMat.refractiveIndex = 1.5f;
		floorMat.shadowcastMode = ShadowcastMode::ALWAYS;
		Renderer::Shape floor = Renderer::Shape(std::make_shared<Math::Plane>(), floorMat);
		floor.transform(Math::Transform().translate(0, -1, 0));
		world.addObject(floor);

		Renderer::Material ballMat;
		ballMat.pattern = std::make_shared<Patterns::SolidColor>(Graphics::Color(1, 0, 0));
		ballMat.ambient = 0.5f;
		Renderer::Shape ball = Renderer::Shape(std::make_shared<Math::Sphere>(), ballMat);
		ball.transform(Math::Transform().translate(0, -3.5f, -0.5f));
		world.addObject(ball);

		auto ray = Math::Ray({ 0,0,-3 }, { 0, -sqrt(2) / 2, sqrt(2) / 2 });
		auto intersections = Math::Intersections({ {sqrt(2), floor} });

		auto cv = ComputedValues(intersections.at(0), ray, intersections);
		auto result = world.shadeHit(cv);

		EXPECT_EQ(result, Graphics::Color(0.93642f, 0.68642f, 0.68642f));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, reflected_color_for_nonreflected_material)
	{
		World world = World::defaultWorld();
		Math::Ray ray = Math::Ray({ 0, 0, 0 }, { 0, 0, 1 });
		Renderer::Shape shape = world.objectAt(1);

		Material mat = Material(shape.material());
		mat.ambient = 1;
		shape.material(mat);
		world.objectAt(1, shape);

		Math::Intersection intersection = Math::Intersection(1, shape);

		ComputedValues cv = ComputedValues(intersection, ray);
		Graphics::Color result = world.reflectedColor(cv);

		EXPECT_EQ(result, Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, reflected_color_for_reflected_material)
	{
		World world = World::defaultWorld();
		auto plane = std::make_shared<Math::Plane>();
		Renderer::Material mat = Renderer::Material();
		mat.reflective = 0.5f;
		Renderer::Shape shape = Renderer::Shape(plane, mat);
		shape.transform(Math::Transform().translate(0, -1, 0));

		world.addObject(shape);
		Math::Ray ray = Math::Ray({ 0, 0, -3 }, { 0, -sqrt(2) / 2, sqrt(2) / 2 });
		Math::Intersection intersection = Math::Intersection(sqrt(2), shape);

		ComputedValues cv = ComputedValues(intersection, ray);
		Graphics::Color result = world.reflectedColor(cv);

		Graphics::Color expectedResult = Graphics::Color(0.190332f, 0.237915f, 0.142749f);

		EXPECT_EQ(result, expectedResult);
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, reflected_color_at_max_recursion_depth)
	{
		World world = World::defaultWorld();
		auto plane = std::make_shared<Math::Plane>();
		Renderer::Material mat = Renderer::Material();
		mat.reflective = 0.5f;
		Renderer::Shape shape = Renderer::Shape(plane, mat);
		shape.transform(Math::Transform().translate(0, -1, 0));

		world.addObject(shape);
		Math::Ray ray = Math::Ray({ 0, 0, -3 }, { 0, -sqrt(2) / 2, sqrt(2) / 2 });
		Math::Intersection intersection = Math::Intersection(sqrt(2), shape);

		ComputedValues cv = ComputedValues(intersection, ray);
		Graphics::Color result = world.reflectedColor(cv, 0);

		Graphics::Color expectedResult = Graphics::Color::black();

		EXPECT_EQ(result, expectedResult);
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, refracted_color_with_opaque_surface)
	{
		World world = World::defaultWorld();
		Shape shape = world.objectAt(0);
		auto ray = Math::Ray({ 0,0,-5 }, { 0,0,1 });
		auto intersections = Math::Intersections({ { 4, shape }, { 6, shape } });
		auto cv = ComputedValues(intersections.at(0), ray, intersections);
		auto result = world.refractedColor(cv);

		EXPECT_EQ(result, Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, refracted_color_at_maximum_recursive_depth)
	{
		World world = World::defaultWorld();
		Shape shape = world.objectAt(0);
		auto mat = shape.material();
		mat.transparency = 1.0f;
		mat.refractiveIndex = 1.5f;
		auto ray = Math::Ray({ 0,0,-5 }, { 0,0,1 });
		auto intersections = Math::Intersections({ { 4, shape }, { 6,shape } });
		auto cv = ComputedValues(intersections.at(0), ray, intersections);
		auto result = world.refractedColor(cv, 0);

		EXPECT_EQ(result, Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, refracted_color_under_total_internal_reflection)
	{
		World world = World::defaultWorld();
		Shape shape = world.objectAt(0);
		auto mat = shape.material();
		mat.transparency = 1.0f;
		mat.refractiveIndex = 1.5f;
		shape.material(mat);
		auto ray = Math::Ray({ 0, 0, sqrt(2) / 2 }, { 0, 1, 0 });
		auto intersections = Math::Intersections({ {-sqrt(2) / 2, shape},{sqrt(2) / 2, shape} });
		auto cv = ComputedValues(intersections.at(1), ray, intersections);
		auto result = world.refractedColor(cv);

		EXPECT_EQ(result, Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, refracted_color_with_refracted_ray)
	{
		World world = World::defaultWorld();
		Shape a = world.objectAt(0);
		auto aMat = a.material();
		aMat.ambient = 1.0f;
		aMat.pattern = std::make_shared<Patterns::TestPattern>();
		a.material(aMat);

		Shape b = world.objectAt(1);
		auto bMat = b.material();
		bMat.transparency = 1.0f;
		bMat.refractiveIndex = 1.5f;
		b.material(bMat);

		world.objectAt(0, a);
		world.objectAt(1, b);

		auto ray = Math::Ray({ 0,0,0.1f }, { 0,1,0 });
		auto intersections = Math::Intersections({ {-0.9899f, a}, {-0.4899f, b}, {0.4899f, b}, {0.9899, a} });
		auto cv = ComputedValues(intersections.at(2), ray, intersections);
		auto result = world.refractedColor(cv);

		EXPECT_EQ(result, Graphics::Color(0, 0.998885f, 0.0472194f));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, reflected_and_refracted_rays)
	{
		World world = World::defaultWorld();
		auto ray = Math::Ray({ 0,0,-3 }, { 0,-sqrt(2) / 2,sqrt(2) / 2 });

		auto floorShape = std::make_shared<Math::Plane>();
		auto floorMaterial = Renderer::Material();
		floorMaterial.reflective = 0.5f;
		floorMaterial.transparency = 0.5f;
		floorMaterial.refractiveIndex = 1.5f;
		floorMaterial.shadowcastMode = ShadowcastMode::ALWAYS;
		auto floor = Renderer::Shape(floorShape, floorMaterial);
		floor.transform(Math::Transform().translate(0, -1, 0));
		world.addObject(floor);

		auto ballShape = std::make_shared<Math::Sphere>();
		auto ballMaterial = Renderer::Material();
		ballMaterial.pattern = std::make_shared<Patterns::SolidColor>(Graphics::Color(1, 0, 0));
		ballMaterial.ambient = 0.5f;
		auto ball = Renderer::Shape(ballShape, ballMaterial);
		ball.transform(Math::Transform().translate(0, -3.5f, -0.5f));
		world.addObject(ball);

		auto intersections = Math::Intersections({ {sqrt(2), floor} });

		auto cv = ComputedValues(intersections.at(0), ray, intersections);
		auto color = world.shadeHit(cv);

		EXPECT_EQ(color, Graphics::Color(0.93391f, 0.69643f, 0.69243f));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, color_at_miss)
	{
		World world = World::defaultWorld();
		Math::Ray ray = Math::Ray({ 0, 0, -5 }, { 0, 1, 0 });
		Graphics::Color color = world.colorAt(ray);

		EXPECT_EQ(color, Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, color_at_hit)
	{
		World world = World::defaultWorld();
		Math::Ray ray = Math::Ray({ 0, 0, -5 }, { 0, 0, 1 });
		Graphics::Color color = world.colorAt(ray);

		EXPECT_EQ(color, Graphics::Color(0.38066f, 0.47583f, 0.2855f));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, color_at_hit_behind_ray)
	{
		World world = World::defaultWorld();
		Math::Ray ray = Math::Ray({ 0, 0, 0.75f }, { 0, 0, -1 });
		Graphics::Color color = world.colorAt(ray);

		EXPECT_EQ(color, Graphics::Color(0.1f, 0.1f, 0.1f));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, color_at_with_mutually_reflective_surface)
	{
		World world = World();
		PointLight light = PointLight({ 0, 0, 0 }, Graphics::Color::white());
		world.addLight(light);

		Material reflectiveMaterial = Material();
		reflectiveMaterial.reflective = 1;

		auto lower = std::make_shared<Math::Plane>();
		lower->transform(Math::Transform().translate(0, -1, 0));

		auto upper = std::make_shared<Math::Plane>();
		upper->transform(Math::Transform().translate(0, 1, 0));

		Renderer::Shape upperShape = Renderer::Shape(upper, reflectiveMaterial);
		Renderer::Shape lowerShape = Renderer::Shape(lower, reflectiveMaterial);

		world.addObject(upperShape);
		world.addObject(lowerShape);

		Math::Ray ray = Math::Ray({ 0, 0, 0 }, { 0, 1, 0 });

		// Infinite recursion gets terminated successfully
		EXPECT_FALSE(world.colorAt(ray) == Graphics::Color::black());
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, is_shadowed_with_nothing_is_colinear_with_point_and_light)
	{
		World world = World::defaultWorld();
		RML::Point position = RML::Point(0, 10, 0);
		const PointLight& light = world.lightAt(0);

		EXPECT_FALSE(world.isShadowed(position, light));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, is_shadowed_with_object_between_point_and_light)
	{
		World world = World::defaultWorld();
		RML::Point position = RML::Point(10, -10, 10);
		const PointLight& light = world.lightAt(0);

		EXPECT_TRUE(world.isShadowed(position, light));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, is_shadowed_with_transparent_object_between_point_and_light)
	{
		World world = World::defaultWorld();
		Shape a = world.objectAt(0);
		auto aMat = a.material();
		aMat.transparency = 0.5f;
		aMat.shadowcastMode = ShadowcastMode::WHEN_TRANSPARENT;
		a.material(aMat);
		world.objectAt(0, a);

		Shape b = world.objectAt(1);
		auto bMat = b.material();
		bMat.transparency = 0.5f;
		bMat.shadowcastMode = ShadowcastMode::WHEN_TRANSPARENT;
		b.material(bMat);
		world.objectAt(1, b);

		RML::Point position = RML::Point(10, -10, 10);
		const PointLight& light = world.lightAt(0);

		EXPECT_FALSE(world.isShadowed(position, light));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, is_shadowed_with_object_behind_light)
	{
		World world = World::defaultWorld();
		RML::Point position = RML::Point(-20, 20, -20);
		const PointLight& light = world.lightAt(0);

		EXPECT_FALSE(world.isShadowed(position, light));
	}

	TEST(CppRayTracerChallenge_Core_Renderer_World, is_shadowed_when_object_behind_point)
	{
		World world = World::defaultWorld();
		RML::Point position = RML::Point(-2, 2, -2);
		const PointLight& light = world.lightAt(0);

		EXPECT_FALSE(world.isShadowed(position, light));
	}
}
