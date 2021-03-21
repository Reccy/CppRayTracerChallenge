#include<gtest/gtest.h>
#include "foo.h"

TEST(FooTest, BarTest)
{
	CppRayTracerChallenge::Core::Foo foo;
	EXPECT_EQ(foo.bar(), true);
};
