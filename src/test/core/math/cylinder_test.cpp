#include <gtest/gtest.h>
#include <math/cylinder.h>

using namespace CppRayTracerChallenge::Core::Math;

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
