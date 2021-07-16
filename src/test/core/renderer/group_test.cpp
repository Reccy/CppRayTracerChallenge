#include <gtest/gtest.h>
#include <renderer/group.h>
#include <renderer/shape.h>
#include <math/transform.h>
#include <math/sphere.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_Group, creating_new_group)
{
	Group group = Group();

	EXPECT_EQ(group.transform(), Math::Transform());
	EXPECT_EQ(group.empty(), true);
	EXPECT_EQ(group.size(), 0);
	EXPECT_EQ(group.count(), 0);
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, adding_child_to_group)
{
	auto group = std::make_shared<Group>();
	auto sphere = std::make_shared<Math::Sphere>();
	auto shape = std::make_shared<Shape>(sphere);

	group->addChild(shape);

	EXPECT_EQ(group->empty(), false);
	EXPECT_EQ(group->size(), 1);
	EXPECT_EQ(group->count(), 1);

	EXPECT_TRUE(group->includes(shape));
	EXPECT_EQ(shape->parent().lock(), group->shared_from_this());
}
