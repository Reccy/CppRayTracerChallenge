#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <renderer/group.h>
#include <renderer/shape.h>
#include <math/transform.h>
#include <math/bounding_box.h>
#include <math/sphere.h>
#include <math/cylinder.h>
#include <math/ray.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

using ::testing::Exactly;
using ::testing::AtLeast;
using ::testing::_;

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

TEST(CppRayTracerChallenge_Core_Renderer_Group, group_has_bounding_box_that_contains_children)
{
	auto mathSphere = std::make_shared<Math::Sphere>();
	auto sphere = std::make_shared<Shape>(mathSphere);

	sphere->transform(Math::Transform()
		.scale(2, 2, 2)
		.translate(2, 5, -3));

	auto mathCylinder = std::make_shared<Math::Cylinder>(-2, 2);
	auto cylinder = std::make_shared<Shape>(mathCylinder);

	cylinder->transform(Math::Transform()
		.scale(0.5, 1, 0.5)
		.translate(-4, -1, 4));

	auto group = Group();
	group.addChild(sphere);
	group.addChild(cylinder);

	Math::BoundingBox box = group.bounds();

	EXPECT_EQ(box.min(), Math::Point(-4.5, -3, -5));
	EXPECT_EQ(box.max(), Math::Point(4, 7, 4.5));
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, intersecting_ray_doesnt_test_children_if_box_is_missed)
{
	class MockSphere : public Math::Sphere
	{
	public:
		MOCK_METHOD(const Math::Intersections, intersect, (Math::Ray ray), (const, override));
	};

	auto mockSphere = std::make_shared<MockSphere>();
	EXPECT_CALL(*mockSphere, intersect(_))
		.Times(Exactly(0));

	auto shape = std::make_shared<Shape>(mockSphere);

	auto group = std::make_shared<Group>();
	group->addChild(shape);

	Math::Ray ray = Math::Ray({ 0, 0, -5 }, { 0, 1, 0 });

	group->intersect(ray);
	
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, intersecting_ray_tests_children_if_box_is_hit)
{
	class MockSphere : public Math::Sphere
	{
	public:
		MOCK_METHOD(const Math::Intersections, intersect, (Math::Ray ray), (const, override));
	};

	auto mockSphere = std::make_shared<MockSphere>();
	EXPECT_CALL(*mockSphere, intersect(_))
		.Times(Exactly(1));

	auto shape = std::make_shared<Shape>(mockSphere);

	auto group = std::make_shared<Group>();
	group->addChild(shape);

	Math::Ray ray = Math::Ray({ 0, 0, -5 }, { 0, 0, 1 });

	group->intersect(ray);
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, partitioning_groups_children)
{
	auto sphere1 = std::make_shared<Math::Sphere>();
	auto sphere2 = std::make_shared<Math::Sphere>();
	auto sphere3 = std::make_shared<Math::Sphere>();

	auto shape1 = std::make_shared<Shape>(sphere1);
	shape1->transform(Math::Transform().translate(-2, 0, 0));

	auto shape2 = std::make_shared<Shape>(sphere2);
	shape2->transform(Math::Transform().translate(2, 0, 0));

	auto shape3 = std::make_shared<Shape>(sphere3);

	Group group = Group();
	group.addChild(shape1);
	group.addChild(shape2);
	group.addChild(shape3);

	auto [left, right] = group.partitionChildren();

	EXPECT_EQ(group.includes(shape1), false);
	EXPECT_EQ(group.includes(shape2), false);
	EXPECT_EQ(group.includes(shape3), true);

	EXPECT_EQ(left.at(0), shape1);
	EXPECT_EQ(right.at(0), shape2);
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, create_subgroup_from_list_of_children)
{
	auto sphere1 = std::make_shared<Math::Sphere>();
	auto sphere2 = std::make_shared<Math::Sphere>();

	auto shape1 = std::make_shared<Shape>(sphere1);
	auto shape2 = std::make_shared<Shape>(sphere2);

	auto group = Group();
	group.makeSubgroup({ shape1, shape2 });

	EXPECT_EQ(group.count(), 1); // Count 1 is the subgroup

	EXPECT_EQ(shape1->parent().lock(), group.weak_from_this().lock());
	EXPECT_EQ(shape2->parent().lock(), group.weak_from_this().lock());

	auto subgroup = group.subgroup(0);

	EXPECT_EQ(subgroup->count(), 2); // Count 2 is the children in the subgroup
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, subdividing_group_partitions_its_children)
{
	auto sphere1 = std::make_shared<Math::Sphere>();
	auto sphere2 = std::make_shared<Math::Sphere>();
	auto sphere3 = std::make_shared<Math::Sphere>();

	auto shape1 = std::make_shared<Shape>(sphere1);
	shape1->transform(Math::Transform()
		.translate(-2, -2, 0));

	auto shape2 = std::make_shared<Shape>(sphere2);
	shape2->transform(Math::Transform()
		.translate(-2, 2, 0));

	auto shape3 = std::make_shared<Shape>(sphere3);
	shape3->transform(Math::Transform()
		.scale(4, 4, 4));

	auto group = std::make_shared<Group>();

	group->addChild(shape1);
	group->addChild(shape2);
	group->addChild(shape3);

	group->divide(1);

	EXPECT_EQ(group->child(0), shape3);
	
	auto subgroup = group->subgroup(0);

	EXPECT_EQ(subgroup->count(), 2);
	EXPECT_EQ(subgroup->subgroup(0)->child(0), shape1);
	EXPECT_EQ(subgroup->subgroup(1)->child(0), shape2);
}

TEST(CppRayTracerChallenge_Core_Renderer_Group, subdividing_group_with_too_few_children)
{
	auto sphere1 = std::make_shared<Math::Sphere>();
	auto sphere2 = std::make_shared<Math::Sphere>();
	auto sphere3 = std::make_shared<Math::Sphere>();
	auto sphere4 = std::make_shared<Math::Sphere>();

	auto shape1 = std::make_shared<Shape>(sphere1);
	shape1->transform(Math::Transform()
		.translate(-2, 0, 0));

	auto shape2 = std::make_shared<Shape>(sphere2);
	shape2->transform(Math::Transform()
		.translate(2, 1, 0));

	auto shape3 = std::make_shared<Shape>(sphere3);
	shape3->transform(Math::Transform()
		.translate(2, -1, 0));

	auto shape4 = std::make_shared<Shape>(sphere4);

	auto group = std::make_shared<Group>();

	group->makeSubgroup({ shape1, shape2, shape3 });

	group->addChild(shape4);

	group->divide(3);

	auto subgroup = group->subgroup(0);

	EXPECT_EQ(group->child(0), shape4);

	EXPECT_EQ(subgroup->count(), 2);

	EXPECT_EQ(subgroup->subgroup(0)->child(0), shape1);
	EXPECT_EQ(subgroup->subgroup(1)->child(0), shape2);
	EXPECT_EQ(subgroup->subgroup(1)->child(1), shape3);
}
