#include <gtest/gtest.h>
#include "renderer/world.h"
#include "math/ray.h"
#include "math/intersections.h"

using namespace CppRayTracerChallenge::Core::Renderer;

TEST(CppRayTracerChallenge_Core_Renderer_World, world_creation)
{
	World w = World();
	EXPECT_EQ(w.objectCount(), 0);
	EXPECT_EQ(w.lightCount(), 0);
}

TEST(CppRayTracerChallenge_Core_Renderer_World, default_world)
{
	World w = World::defaultWorld();
	EXPECT_EQ(w.lightCount(), 1);
	EXPECT_EQ(w.objectCount(), 2);
}

TEST(CppRayTracerChallenge_Core_Renderer_World, intersect_ray)
{
	World w = World::defaultWorld();
	Math::Ray r = Math::Ray({ 0, 0, -5 }, { 0,0,1 });

	Math::Intersections intersections = w.intersectRay(r);

	EXPECT_EQ(intersections.count(), 4);
	EXPECT_EQ(intersections.at(0).t(), 4);
	EXPECT_EQ(intersections.at(1).t(), 4.5);
	EXPECT_EQ(intersections.at(2).t(), 5.5);
	EXPECT_EQ(intersections.at(3).t(), 6);
}
