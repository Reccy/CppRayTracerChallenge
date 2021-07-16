#include <gtest/gtest.h>
#include <renderer/group.h>

using namespace CppRayTracerChallenge::Core::Renderer;

TEST(CppRayTracerChallenge_Core_Renderer_Group, creating_new_group)
{
	Group group = Group();

	EXPECT_EQ(group.transform(), Transform());
	EXPECT_EQ(group.empty(), true);
	EXPECT_EQ(group.size(), 0);
	EXPECT_EQ(group.count(), 0);
}
