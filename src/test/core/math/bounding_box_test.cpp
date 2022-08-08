#include <gtest/gtest.h>
#include <math/sphere.h>
#include <math/bounding_box.h>
#include <math/ray.h>
#include <RML.h>

namespace CppRayTracerChallenge::Core::Math
{
	using RML::INF;
	using RML::Trig::PI;

	TEST(CppRayTracerChallenge_Core_Math_BoundingBox, create_empty_bounding_box)
	{
		BoundingBox box = BoundingBox();

		Point expectedMin = Point(INF, INF, INF);
		Point expectedMax = Point(-INF, -INF, -INF);

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

	TEST(CppRayTracerChallenge_Core_Math_BoundingBox, transforming_a_bounding_box)
	{
		BoundingBox box = BoundingBox({ -1,-1,-1 }, { 1,1,1 });

		box.transform(Transform()
			.rotate(0, RML::Trig::radians_to_degrees(PI / 4), 0)
			.rotate(RML::Trig::radians_to_degrees(PI / 4), 0, 0));

		Point expectedMin = Point(-1.41421, -1.70711, -1.70711);
		Point expectedMax = Point(1.41421, 1.70711, 1.70711);

		EXPECT_EQ(box.min(), expectedMin);
		EXPECT_EQ(box.max(), expectedMax);
	}

	TEST(CppRayTracerChallenge_Core_Math_Bounding_box, intersecting_ray_with_bounding_box_at_origin)
	{
		struct Param
		{
			Param(Point origin, Vector direction, bool result)
				: origin(origin), direction(direction), result(result) {};

			Point origin;
			Vector direction;
			bool result;
		};

		std::vector<Param> paramsList = {
			{{5, 0.5, 0}, {-1, 0, 0}, true},
			{{-5, 0.5, 0}, {1, 0, 0}, true},
			{{0.5, 5, 0}, {0, -1, 0}, true},
			{{0.5, -5, 0}, {0, 1, 0}, true},
			{{0.5, 0, 5}, {0, 0, -1}, true},
			{{0.5, 0, -5}, {0, 0, 1}, true},
			{{0, 0.5, 0}, {0, 0, 1}, true},
			{{-2, 0, 0}, {2, 4, 6}, false},
			{{0, -2, 0}, {6, 2, 4}, false},
			{{0, 0, -2}, {4, 6, 2}, false},
			{{2, 0, 2}, {0, 0, -1}, false},
			{{0, 2, 2}, {0, -1, 0}, false},
			{{2, 2, 0}, {-1, 0, 0}, false}
		};

		BoundingBox box = BoundingBox({ -1, -1, -1 }, { 1, 1, 1 });

		for (int i = 0; i < paramsList.size(); ++i)
		{
			const Param& param = paramsList[i];

			Vector dir = param.direction.normalized();
			Ray ray = Ray(param.origin, dir);

			EXPECT_EQ(box.intersects(ray), param.result);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Bounding_box, intersecting_ray_with_non_cubic_bounding_box)
	{
		struct Param
		{
			Param(Point origin, Vector direction, bool result)
				: origin(origin), direction(direction), result(result) {};

			Point origin;
			Vector direction;
			bool result;
		};

		std::vector<Param> paramsList = {
			{{15, 1, 2}, {-1, 0, 0}, true},
			{{-5, -1, 4}, {1, 0, 0}, true},
			{{7, 6, 5}, {0, -1, 0}, true},
			{{9, -5, 6}, {0, 1, 0}, true},
			{{8, 2, 12}, {0, 0, -1}, true},
			{{6, 0, -5}, {0, 0, 1}, true},
			{{8, 1, 3.5}, {0, 0, 1}, true},
			{{9, -1, -8}, {2, 4, 6}, false},
			{{8, 3, -4}, {6, 2, 4}, false},
			{{9, -1, -2}, {4, 6, 2}, false},
			{{4, 0, 9}, {0, 0, -1}, false},
			{{8, 6, -1}, {0, -1, 0}, false},
			{{12, 5, 4}, {-1, 0, 0,}, false}
		};

		BoundingBox box = BoundingBox({ 5, -2, 0 }, { 11, 4, 7 });

		for (int i = 0; i < paramsList.size(); ++i)
		{
			const Param& param = paramsList[i];

			Vector dir = param.direction.normalized();
			Ray ray = Ray(param.origin, dir);

			EXPECT_EQ(box.intersects(ray), param.result);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_BoundingBox, splitting_perfect_cube)
	{
		BoundingBox box = BoundingBox({ -1, -4, -5 }, { 9, 6, 5 });
		auto [left, right] = box.split();

		EXPECT_EQ(left.min(), Point(-1, -4, -5));
		EXPECT_EQ(left.max(), Point(4, 6, 5));
		EXPECT_EQ(right.min(), Point(4, -4, -5));
		EXPECT_EQ(right.max(), Point(9, 6, 5));
	}

	TEST(CppRayTracerChallenge_Core_Math_BoundingBox, splitting_x_wide_box)
	{
		BoundingBox box = BoundingBox({ -1, -2, -3 }, { 9, 5.5, 3 });
		auto [left, right] = box.split();

		EXPECT_EQ(left.min(), Point(-1, -2, -3));
		EXPECT_EQ(left.max(), Point(4, 5.5, 3));
		EXPECT_EQ(right.min(), Point(4, -2, -3));
		EXPECT_EQ(right.max(), Point(9, 5.5, 3));
	}

	TEST(CppRayTracerChallenge_Core_Math_BoundingBox, splitting_y_wide_box)
	{
		BoundingBox box = BoundingBox({ -1, -2, -3 }, { 5, 8, 3 });
		auto [left, right] = box.split();

		EXPECT_EQ(left.min(), Point(-1, -2, -3));
		EXPECT_EQ(left.max(), Point(5, 3, 3));
		EXPECT_EQ(right.min(), Point(-1, 3, -3));
		EXPECT_EQ(right.max(), Point(5, 8, 3));
	}

	TEST(CppRayTracerChallenge_Core_Math_BoundingBox, splitting_z_wide_box)
	{
		BoundingBox box = BoundingBox({ -1, -2, -3 }, { 5, 3, 7 });
		auto [left, right] = box.split();

		EXPECT_EQ(left.min(), Point(-1, -2, -3));
		EXPECT_EQ(left.max(), Point(5, 3, 2));
		EXPECT_EQ(right.min(), Point(-1, -2, 2));
		EXPECT_EQ(right.max(), Point(5, 3, 7));
	}
}
