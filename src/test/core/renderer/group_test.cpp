#include <gtest/gtest.h>
#include <renderer/group.h>
#include <renderer/shape.h>
#include <math/transform.h>
#include <math/sphere.h>
#include <math/ray.h>

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

TEST(CppRayTracerChallenge_Core_Renderer_Group, intersecting_ray_with_empty_group)
{
	auto group = std::make_shared<Group>();
	auto ray = Math::Ray({ 0, 0, 0 }, { 0, 0, 1 });
	auto intersections = group->intersectLocal(ray);

	EXPECT_EQ(intersections.count(), 0);
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, intersecting_ray_with_nonempty_group)
{
	auto group = std::make_shared<Group>();
	auto s1 = std::make_shared<Math::Sphere>();
	auto s2 = std::make_shared<Math::Sphere>();
	auto s3 = std::make_shared<Math::Sphere>();

	s2->transform(Math::Transform().translate(0, 0, -3));
	s3->transform(Math::Transform().translate(5, 0, 0));

	auto s1R = std::make_shared<Renderer::Shape>(s1);
	auto s2R = std::make_shared<Renderer::Shape>(s2);
	auto s3R = std::make_shared<Renderer::Shape>(s3);

	group->addChild(s1R);
	group->addChild(s2R);
	group->addChild(s3R);

	auto ray = Math::Ray({ 0, 0, -5 }, { 0, 0, 1 });
	auto intersections = group->intersectLocal(ray);

	EXPECT_EQ(intersections.count(), 4);
	EXPECT_EQ(static_cast<const Shape&>(intersections.at(0).shape()), *s2R);
	EXPECT_EQ(static_cast<const Shape&>(intersections.at(1).shape()), *s2R);
	EXPECT_EQ(static_cast<const Shape&>(intersections.at(2).shape()), *s1R);
	EXPECT_EQ(static_cast<const Shape&>(intersections.at(3).shape()), *s1R);
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, intersecting_transformed_group)
{
	auto group = std::make_shared<Group>();
	group->transform(Math::Transform().scale(2, 2, 2));

	auto sphere = std::make_shared<Math::Sphere>();
	auto shape = std::make_shared<Shape>(sphere);
	shape->transform(Math::Transform().translate(5, 0, 0));

	group->addChild(shape);

	auto ray = Math::Ray({ 10, 0, -10 }, { 0, 0, 1 });
	auto intersections = group->intersect(ray);

	EXPECT_EQ(intersections.count(), 2);
}
