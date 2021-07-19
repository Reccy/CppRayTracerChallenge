#include <gtest/gtest.h>
#include <math/constants.h>
#include <renderer/bounding_box.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_BoundingBox, create_empty_bounding_box)
{
	BoundingBox box = BoundingBox();

	Point expectedMin = Point({ INF, INF, INF });
	Point expectedMax = Point({ -INF, -INF, -INF });

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}

TEST(CppRayTracerChallenge_Core_Renderer_BoundingBox, create_with_volume)
{
	BoundingBox box = BoundingBox({ -1, -2, -3 }, { 3,2,1 });

	Point expectedMin = Point(-1, -2, -3);
	Point expectedMax = Point(3, 2, 1);

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}
