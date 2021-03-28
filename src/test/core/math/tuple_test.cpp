#include <gtest/gtest.h>
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core::Math;

TEST(CppRayTracerChallenge_Core_Math_Tuple, tuple_with_w1_is_a_point)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_EQ(tuple.getX(), 4.3f);
	EXPECT_EQ(tuple.getY(), -4.2f);
	EXPECT_EQ(tuple.getZ(), 3.1f);
	EXPECT_EQ(tuple.getW(), 1.0);
	EXPECT_EQ(tuple.isPoint(), true);
	EXPECT_EQ(tuple.isVector(), false);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, tuple_with_w0_is_a_vector)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 0.0f);
	EXPECT_EQ(tuple.getX(), 4.3f);
	EXPECT_EQ(tuple.getY(), -4.2f);
	EXPECT_EQ(tuple.getZ(), 3.1f);
	EXPECT_EQ(tuple.getW(), 0.0f);
	EXPECT_EQ(tuple.isPoint(), false);
	EXPECT_EQ(tuple.isVector(), true);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, buildPoint_will_build_tuple_with_w1)
{
	Tuple point = Tuple::buildPoint(4.3f, -4.2f, 3.1f);
	Tuple tuple(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_EQ(point, tuple);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, buildVector_will_build_tuple_with_w0)
{
	Tuple vector = Tuple::buildVector(4.3f, -4.2f, 3.1f);
	Tuple tuple(4.3f, -4.2f, 3.1f, 0.0f);
	EXPECT_EQ(vector, tuple);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_true)
{
	Tuple tupleA(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple tupleB(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_TRUE(tupleA == tupleB);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, equality_false)
{
	Tuple tupleA(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple tupleB(1.0f, 2.0f, -9.3f, 0.0f);
	EXPECT_FALSE(tupleA == tupleB);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_true)
{
	Tuple tupleA(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple tupleB(1.0f, 2.0f, -9.3f, 0.0f);
	EXPECT_TRUE(tupleA != tupleB);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, inequality_false)
{
	Tuple tupleA(4.3f, -4.2f, 3.1f, 1.0f);
	Tuple tupleB(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_FALSE(tupleA != tupleB);
}

TEST(CppRayTracerChallenge_Core_Math_Tuple, ostream_will_show_x_y_z_w_components)
{
	Tuple tuple(4.3f, -4.2f, 3.1f, 1.0f);
	std::stringstream ss;
	ss << tuple;

	EXPECT_EQ(ss.str(), "4.3, -4.2, 3.1, 1");
}
