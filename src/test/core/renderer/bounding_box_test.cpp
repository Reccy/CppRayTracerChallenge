#include <gtest/gtest.h>
#include <renderer/bounding_box.h>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Renderer_BoundingBox, create_empty_bounding_box)
{
	BoundingBox box = BoundingBox();

	Math::Point expectedMin = Math::Point({ INFINITY, INFINITY, INFINITY });
	Math::Point expectedMax = Math::Point({ -INFINITY, -INFINITY, -INFINITY });

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}
