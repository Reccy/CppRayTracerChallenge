#include <gtest/gtest.h>
#include <renderer/csg.h>
#include <math/sphere.h>
#include <math/cube.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using CppRayTracerChallenge::Core::Math::Cube;
using CppRayTracerChallenge::Core::Math::Sphere;

TEST(CppRayTracerChallenge_Core_Renderer_CSG, csg_is_created_with_an_operation_and_two_shapes)
{
	auto sphere = std::make_shared<Sphere>();
	auto cube = std::make_shared<Cube>();

	auto s1 = std::make_shared<Shape>(sphere);
	auto s2 = std::make_shared<Shape>(cube);

	auto csg = CSG(CSG::Operation::UNION, s1, s2);

	EXPECT_EQ(csg.operation(), CSG::Operation::UNION);
	EXPECT_EQ(csg.left(), s1);
	EXPECT_EQ(csg.right(), s2);
	EXPECT_EQ(s1->parent(), csg);
	EXPECT_EQ(s2->parent(), csg);
}
