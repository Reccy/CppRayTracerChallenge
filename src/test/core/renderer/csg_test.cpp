#include <gtest/gtest.h>
#include <array>
#include <renderer/csg.h>
#include <math/sphere.h>
#include <math/cube.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using CppRayTracerChallenge::Core::Math::Cube;
using CppRayTracerChallenge::Core::Math::Sphere;

constexpr CSG::Operation UNION = CSG::Operation::UNION;
constexpr CSG::Operation INTERSECT = CSG::Operation::INTERSECT;
constexpr CSG::Operation DIFFERENCE = CSG::Operation::DIFFERENCE;

struct CSGRuleEval
{
	CSGRuleEval(CSG::Operation op, bool lhit, bool inl, bool inr, bool result)
		: op(op), lhit(lhit), inl(inl), inr(inr), result(result) {};

	CSG::Operation op;
	bool lhit;
	bool inl;
	bool inr;
	bool result;
};

TEST(CppRayTracerChallenge_Core_Renderer_CSG, csg_is_created_with_an_operation_and_two_shapes)
{
	auto sphere = std::make_shared<Sphere>();
	auto cube = std::make_shared<Cube>();

	auto s1 = std::make_shared<Shape>(sphere);
	auto s2 = std::make_shared<Shape>(cube);

	auto csg = CSG::build(UNION, s1, s2);

	EXPECT_EQ(csg->operation(), UNION);
	EXPECT_EQ(csg->left(), s1);
	EXPECT_EQ(csg->right(), s2);
	EXPECT_EQ(std::dynamic_pointer_cast<CSG>(s1->parent().lock()), csg);
	EXPECT_EQ(std::dynamic_pointer_cast<CSG>(s2->parent().lock()), csg);
}

TEST(CppRayTracerChallenge_Core_Renderer_CSG, evaluation_for_csg_rule)
{
	std::array<CSGRuleEval, 8> ruleTable = {{
		{ UNION, true, true, true, false },
		{ UNION, true, true, false, true },
		{ UNION, true, false, true, false },
		{ UNION, true, false, false, true },
		{ UNION, false, true, true, false },
		{ UNION, false, true, false, false },
		{ UNION, false, false, true, true },
		{ UNION, false, false, false, true }
	}};

	for (auto eval : ruleTable)
	{
		bool result = CSG::intersectionAllowed(eval.op, eval.lhit, eval.inl, eval.inr);
		EXPECT_EQ(result, eval.result);
	}
}
