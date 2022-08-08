#include <gtest/gtest.h>
#include <math/cone.h>
#include <math/comparison.h>

namespace CppRayTracerChallenge::Core::Math
{
	using RML::INF;

	TEST(CppRayTracerChallenge_Core_Math_Cone, default_attributes)
	{
		Cone a = Cone();
		EXPECT_EQ(a.maximum(), INFINITY);
		EXPECT_EQ(a.minimum(), -INFINITY);
		EXPECT_EQ(a.closed(), false);

		Cone b = Cone(-0.5, 0.5);
		EXPECT_EQ(b.minimum(), -0.5);
		EXPECT_EQ(b.maximum(), 0.5);
		EXPECT_EQ(b.closed(), false);

		Cone c = Cone(-0.5, 0.5, true);
		EXPECT_EQ(c.minimum(), -0.5);
		EXPECT_EQ(c.maximum(), 0.5);
		EXPECT_EQ(c.closed(), true);
	}

	TEST(CppRayTracerChallenge_Core_Math_Cone, intersecting_cone_with_ray)
	{
		struct Param
		{
			Param(Point origin, Vector direction, double t0, double t1)
				: origin(origin), direction(direction), t0(t0), t1(t1) {};

			Point origin;
			Vector direction;
			double t0;
			double t1;
		};

		std::vector<Param> paramsList
		{
			Param({0, 0, -5}, {0, 0, 1}, 5, 5),
			Param({0, 0, -5}, {1, 1, 1}, 8.66025, 8.66025),
			Param({1, 1, -5}, {-0.5, -1, 1}, 4.55006, 49.44994)
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Param& param = paramsList[i];
			Cone cone = Cone();

			Vector direction = param.direction.normalize();
			Ray ray = Ray(param.origin, direction);

			Intersections intersections = cone.intersectLocal(ray);

			EXPECT_EQ(intersections.count(), 2);
			EXPECT_TRUE(Comparison::equal(intersections.at(0).t(), param.t0));
			EXPECT_TRUE(Comparison::equal(intersections.at(1).t(), param.t1));
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cone, intersecting_cone_with_ray_parallel_to_one_of_its_halves)
	{
		Cone cone = Cone();
		Vector direction = Vector(0, 1, 1).normalize();
		Ray ray = Ray({ 0, 0, -1 }, direction);

		Intersections result = cone.intersectLocal(ray);

		EXPECT_EQ(result.count(), 1);
		EXPECT_TRUE(Comparison::equal(result.at(0).t(), 0.35355));
	}

	TEST(CppRayTracerChallenge_Core_Math_Cone, intersecting_cone_end_caps)
	{
		struct Param
		{
			Param(Point origin, Vector direction, int count)
				: origin(origin), direction(direction), count(count) {};

			Point origin;
			Vector direction;
			int count;
		};

		std::vector<Param> paramsList
		{
			Param({0, 0, -5}, {0, 1, 0}, 0),
			Param({0, 0, -0.25}, {0, 1, 1}, 2),
			Param({0, 0, -0.25}, {0, 1, 0}, 4)
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Param& param = paramsList[i];

			Cone cone = Cone(-0.5, 0.5, true);

			Vector direction = param.direction.normalize();
			Ray ray = Ray(param.origin, direction);
			Intersections result = cone.intersectLocal(ray);

			EXPECT_EQ(result.count(), param.count);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cone, computing_normal_vector_on_cone)
	{
		struct Param
		{
			Param(Point position, Vector normal)
				: position(position), normal(normal) {};

			Point position;
			Vector normal;
		};

		std::vector<Param> paramsList
		{
			Param({0, 0, 0}, {0, 0, 0}),
			Param({1, 1, 1}, {1, -sqrt(2), 1}),
			Param({-1, -1, 0}, {-1, 1, 0})
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Param& param = paramsList[i];

			Cone cone = Cone();
			Vector result = cone.normalLocal(param.position);

			EXPECT_EQ(result, param.normal);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cone, unbound_cone_has_bounding_box)
	{
		Cone cone = Cone();
		BoundingBox box = cone.bounds();

		Point expectedMin = Point(-INF, -INF, -INF);
		Point expectedMax = Point(INF, INF, INF);

		EXPECT_EQ(box.min(), expectedMin);
		EXPECT_EQ(box.max(), expectedMax);
	}

	TEST(CppRayTracerChallenge_Core_Math_Cone, bound_cone_has_bounding_box)
	{
		Cone cone = Cone(-5, 3);
		BoundingBox box = cone.bounds();

		Point expectedMin = Point(-5, -5, -5);
		Point expectedMax = Point(5, 3, 5);

		EXPECT_EQ(box.min(), expectedMin);
		EXPECT_EQ(box.max(), expectedMax);
	}
}
