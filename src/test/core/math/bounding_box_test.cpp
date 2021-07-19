#include <gtest/gtest.h>
#include <math/constants.h>
#include <math/bounding_box.h>

using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Math_BoundingBox, create_empty_bounding_box)
{
	BoundingBox box = BoundingBox();

	Point expectedMin = Point({ INF, INF, INF });
	Point expectedMax = Point({ -INF, -INF, -INF });

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}

TEST(CppRayTracerChallenge_Core_Math_BoundingBox, create_with_volume)
{
	BoundingBox box = BoundingBox({ -1, -2, -3 }, { 3,2,1 });

	Point expectedMin = Point(-1, -2, -3);
	Point expectedMax = Point(3, 2, 1);

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}

TEST(CppRayTracerChallenge_Core_Math_BoundingBox, add_points_to_empty_bounding_box)
{
	BoundingBox box = BoundingBox();

	Point p1 = Point(-5, 2, 0);
	Point p2 = Point(7, 0, -3);

	box.add(p1);
	box.add(p2);

	Point expectedMin = Point(-5, 0, -3);
	Point expectedMax = Point(7, 2, 0);

	EXPECT_EQ(box.min(), expectedMin);
	EXPECT_EQ(box.max(), expectedMax);
}

TEST(CppRayTracerChallenge_Core_Math_BoundingBox, add_bounding_box_to_another)
{
	BoundingBox box1 = BoundingBox({ -5, -2, 0 }, { 7, 4, 4 });
	BoundingBox box2 = BoundingBox({ 8, -7, -2 }, { 14, 2, 8 });

	box1.add(box2);

	Point expectedMin = Point(-5, -7, -2);
	Point expectedMax = Point(14, 4, 8);

	EXPECT_EQ(box1.min(), expectedMin);
	EXPECT_EQ(box1.max(), expectedMax);
}



TEST(CppRayTracerChallenge_Core_Math_BoundingBox, check_to_see_if_bounding_box_contains_point)
{
	struct Param
	{
		Param(Point point, bool result)
			: point(point), result(result) {};

		Point point;
		bool result;
	};

	std::vector<Param> paramsList = {
		{ Point(5, -2, 0), true },
		{ Point(11, 4, 7), true },
		{ Point(8, 1, 3), true },
		{ Point(3, 0, 3), false },
		{ Point(8, -4, 3), false },
		{ Point(8, 1, -1), false },
		{ Point(13, 1, 3), false },
		{ Point(8, 5, 3), false },
		{ Point(8, 1, 8), false }
	};

	BoundingBox box = BoundingBox({ 5, -2, 0 }, { 11, 4, 7 });

	for (int i = 0; i < paramsList.size(); ++i)
	{
		const Param& param = paramsList[i];

		bool result = box.contains(param.point);

		EXPECT_EQ(result, param.result);
	}
}

TEST(CppRayTracerChallenge_Core_Math_BoundingBox, check_to_see_if_box_contains_other_box)
{
	struct Param
	{
		Param(BoundingBox box, bool result)
			: box(box), result(result) {};

		BoundingBox box;
		bool result;
	};

	std::vector<Param> paramsList = {
		{ { Point(5, -2, 0), Point(11, 4, 7) }, true },
		{ { Point(6, -1, 1), Point(10, 3, 6) }, true },
		{ { Point(4, -3, -1), Point(10, 3, 6) }, false },
		{ { Point(6, -1, 1), Point(12, 5, 8) }, false }
	};

	BoundingBox box = BoundingBox({ 5, -2, 0 }, { 11,4,7 });

	for (int i = 0; i < paramsList.size(); ++i)
	{
		const Param& param = paramsList[i];

		bool result = box.contains(param.box);

		EXPECT_EQ(result, param.result);
	}
}
