#include <gtest/gtest.h>
#include "renderer/world.h"

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
