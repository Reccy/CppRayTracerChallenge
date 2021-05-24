#include <gtest/gtest.h>
#include "renderer/world.h"
#include "renderer/computed_values.h"
#include "math/ray.h"
#include "math/intersections.h"
#include "math/sphere.h"

using namespace CppRayTracerChallenge::Core::Renderer;

TEST(CppRayTracerChallenge_Core_Renderer_World, world_creation)
{
	World world = World();
	EXPECT_EQ(world.objectCount(), 0);
	EXPECT_EQ(world.lightCount(), 0);
}

TEST(CppRayTracerChallenge_Core_Renderer_World, default_world)
{
	World world = World::defaultWorld();

	const Sphere& obj1 = world.objectAt(0);
	const Sphere& obj2 = world.objectAt(1);
	const PointLight& light1 = world.lightAt(0);

	Material s1mat = Material();
	s1mat.color = Graphics::Color(0.8f, 1.0f, 0.6f);
	s1mat.diffuse = 0.7f;
	s1mat.specular = 0.2f;

	EXPECT_EQ(obj1.material(), s1mat);

	Math::Transform s2transform = Math::Transform()
		.scale(0.5, 0.5, 0.5);

	EXPECT_EQ(obj2.transform(), s2transform);

	EXPECT_EQ(light1.position(), Math::Point(-10, 10, -10));
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