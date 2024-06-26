#include <gtest/gtest.h>
#include <math/cylinder.h>
#include <math/comparison.h>

namespace CppRayTracerChallenge::Core::Math
{
	using RML::INF;

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, ray_misses_cylinder)
	{
		struct Param
		{
			Param(Point origin, Vector direction) : origin(origin), direction(direction) {};

			Point origin;
			Vector direction;
		};

		std::vector<Param> paramsList{
			Param({ 1, 0, 0 }, { 0, 1, 0 }),
			Param({ 0, 0, 0 },{ 0, 1, 0 }),
			Param({ 0, 0, -5 },{ 1, 1, 1 })
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Cylinder cylinder = Cylinder();
			Param& param = paramsList[i];

			Vector direction = param.direction.normalize();
			Ray ray = Ray(param.origin, direction);

			Intersections intersections = cylinder.intersectLocal(ray);

			EXPECT_EQ(intersections.count(), 0);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, ray_hits_cylinder)
	{
		struct Param
		{
			Param(Point origin, Vector direction, double t0, double t1) : origin(origin), direction(direction), t0(t0), t1(t1) {};

			Point origin;
			Vector direction;
			double t0;
			double t1;
		};

		std::vector<Param> paramsList{
			Param({ 1, 0, -5 }, { 0, 0, 1 }, 5, 5),
			Param({ 0, 0, -5 }, { 0, 0, 1 }, 4, 6),
			Param({ 0.5, 0, -5 }, { 0.1, 1, 1 }, 6.80798, 7.08872)
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Cylinder cylinder = Cylinder();
			Param& param = paramsList[i];

			Vector direction = param.direction.normalize();
			Ray ray = Ray(param.origin, direction);

			Intersections intersections = cylinder.intersectLocal(ray);

			EXPECT_EQ(intersections.count(), 2);
			EXPECT_TRUE(Comparison::equal(intersections.at(0).t(), param.t0));
			EXPECT_TRUE(Comparison::equal(intersections.at(1).t(), param.t1));
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, normal_vector_on_cylinder)
	{
		struct Param
		{
			Param(Point position, Vector normal) : position(position), normal(normal) {};

			Point position;
			Vector normal;
		};

		std::vector<Param> paramsList{
			Param({ 1, 0, 0 }, { 1, 0, 0 }),
			Param({ 0, 5, -1 }, { 0, 0, -1 }),
			Param({ 0, -2, 1 }, { 0, 0, 1 }),
			Param({ -1, 1, 0 }, { -1, 0, 0 })
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Cylinder cylinder = Cylinder();
			Param& param = paramsList[i];

			Vector normal = cylinder.normalLocal(param.position);

			EXPECT_EQ(normal, param.normal);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, default_minimum_and_maximum_bounds)
	{
		Cylinder cylinder = Cylinder();
		EXPECT_EQ(cylinder.maximum(), INFINITY);
		EXPECT_EQ(cylinder.minimum(), -INFINITY);
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, intersecting_constrained_cylinder)
	{
		struct Param
		{
			Param(Point origin, Vector direction, int count)
				: origin(origin), direction(direction), count(count) {};

			Point origin;
			Vector direction;
			int count;
		};

		std::vector<Param> paramsList{
			Param({0, 1.5, 0}, {0.1, 1, 0}, 0),
			Param({0, 3, -5}, {0, 0, 1}, 0),
			Param({0, 0, -5}, {0, 0, 1}, 0),
			Param({0, 2, -5}, {0, 0, 1}, 0),
			Param({0, 1, -5}, {0, 0, 1}, 0),
			Param({0, 1.5, -2}, {0, 0, 1}, 2)
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Param& param = paramsList[i];

			Cylinder cylinder = Cylinder(1, 2);
			Vector direction = param.direction.normalize();
			Ray ray = Ray(param.origin, direction);

			Intersections intersections = cylinder.intersectLocal(ray);

			EXPECT_EQ(intersections.count(), param.count);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, default_closed_attribute)
	{
		Cylinder cylinder = Cylinder();
		EXPECT_EQ(cylinder.closed(), false);
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, intersecting_caps_of_closed_cylinder)
	{
		struct Param
		{
			Param(Point origin, Vector direction, int count)
				: origin(origin), direction(direction), count(count) {};

			Point origin;
			Vector direction;
			int count;
		};

		std::vector<Param> paramsList{
			Param({0, 3, 0}, {0, -1, 0}, 2),
			Param({0, 3, -2}, {0, -1, 2}, 2),
			Param({0, 4, -2}, {0, -1, 1}, 2),
			Param({0, 0, -2}, {0, 1, 2}, 2),
			Param({0, -1, -2}, {0, 1, 1}, 2)
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Param& param = paramsList[i];

			Cylinder cylinder = Cylinder(1, 2, true);
			Vector direction = param.direction.normalize();
			Ray ray = Ray(param.origin, direction);

			Intersections intersections = cylinder.intersectLocal(ray);

			EXPECT_EQ(intersections.count(), param.count);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, normal_vector_on_caps)
	{
		struct Param
		{
			Param(Point position, Vector normal)
				: position(position), normal(normal) {};

			Point position;
			Vector normal;
		};

		std::vector<Param> paramsList{
			Param({0, 1, 0}, {0, -1, 0}),
			Param({0.5, 1, 0}, {0, -1, 0}),
			Param({0, 1, 0.5}, {0, -1, 0}),
			Param({0, 2, 0}, {0, 1, 0}),
			Param({0.5, 2, 0}, {0, 1, 0}),
			Param({0, 2, 0.5}, {0, 1, 0})
		};

		for (int i = 0; i < paramsList.size(); ++i)
		{
			Param& param = paramsList[i];
			Cylinder cylinder = Cylinder(1, 2, true);

			Vector result = cylinder.normalLocal(param.position);

			EXPECT_EQ(result, param.normal);
		}
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, unbound_cylinder_has_bounding_box)
	{
		Cylinder cylinder = Cylinder();
		BoundingBox box = cylinder.bounds();

		Point expectedMin = Point(-1, -INF, -1);
		Point expectedMax = Point(1, INF, 1);

		EXPECT_EQ(box.min(), expectedMin);
		EXPECT_EQ(box.max(), expectedMax);
	}

	TEST(CppRayTracerChallenge_Core_Math_Cylinder, bound_cylinder_has_bounding_box)
	{
		Cylinder cylinder = Cylinder(-5, 3);
		BoundingBox box = cylinder.bounds();

		Point expectedMin = Point(-1, -5, -1);
		Point expectedMax = Point(1, 3, 1);

		EXPECT_EQ(box.min(), expectedMin);
		EXPECT_EQ(box.max(), expectedMax);
	}
}
