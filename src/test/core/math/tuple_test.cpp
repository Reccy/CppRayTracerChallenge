#include <gtest/gtest.h>
#include "math/tuple.h"

TEST(TupleTest, TupleWithW0IsAPoint)
{
	CppRayTracerChallenge::Core::Math::Tuple tuple(4.3f, -4.2f, 3.1f, 1.0f);
	EXPECT_EQ(tuple.getX(), 4.3f);
	EXPECT_EQ(tuple.getY(), -4.2f);
	EXPECT_EQ(tuple.getZ(), 3.1f);
	EXPECT_EQ(tuple.getW(), 1.0);
	EXPECT_EQ(tuple.isPoint(), true);
	EXPECT_EQ(tuple.isVector(), false);
}

TEST(TupleTest, TupleWithW1IsAVector)
{
	CppRayTracerChallenge::Core::Math::Tuple tuple(4.3f, -4.2f, 3.1f, 0.0f);
	EXPECT_EQ(tuple.getX(), 4.3f);
	EXPECT_EQ(tuple.getY(), -4.2f);
	EXPECT_EQ(tuple.getZ(), 3.1f);
	EXPECT_EQ(tuple.getW(), 0.0f);
	EXPECT_EQ(tuple.isPoint(), false);
	EXPECT_EQ(tuple.isVector(), true);
}
