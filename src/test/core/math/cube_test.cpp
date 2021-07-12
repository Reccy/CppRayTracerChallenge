#include <gtest/gtest.h>
#include <math/cube.h>

using namespace CppRayTracerChallenge::Core::Math;

class RayIntersectsCubeParam
{
public:
	RayIntersectsCubeParam(std::string caseName, Point origin, Vector direction, float t1, float t2) : caseName(caseName), origin(origin), direction(direction), t1(t1), t2(t2) {};

	std::string caseName;
	Point origin;
	Vector direction;
	float t1;
	float t2;

	friend std::ostream& operator<<(std::ostream& os, const RayIntersectsCubeParam& param)
	{
		os << param.caseName << ": origin(" << param.origin << "), dir(" << param.direction << "), t1(" << param.t1 << "), t2(" << param.t2 << ")";
		return os;
	};
};

class CppRayTracerChallenge_Core_Math_Cube : public testing::TestWithParam<RayIntersectsCubeParam> {};

TEST_P(CppRayTracerChallenge_Core_Math_Cube, ray_intersects_cube)
{
	RayIntersectsCubeParam params = GetParam();

	Cube cube = Cube();
	Ray ray = Ray(params.origin, params.direction);
	Intersections intersections = cube.intersectLocal(ray);

	EXPECT_EQ(intersections.count(), 2);
	EXPECT_EQ(intersections.at(0).t(), params.t1);
	EXPECT_EQ(intersections.at(1).t(), params.t2);
}

INSTANTIATE_TEST_SUITE_P(
	CppRayTracerChallenge_Core_Math_Cube_Tests,
	CppRayTracerChallenge_Core_Math_Cube,
	::testing::Values(
		RayIntersectsCubeParam("+x", { 5, 0.5, 0 }, { -1, 0, 0 }, 4, 6),
		RayIntersectsCubeParam("-x", { -5, 0.5, 0 }, { 1, 0, 0 }, 4, 6),
		RayIntersectsCubeParam("+y", { 0.5, 5, 0 }, { 0, -1, 0 }, 4, 6),
		RayIntersectsCubeParam("-y", { 0.5, -5, 0 }, { 0, 1, 0 }, 4, 6),
		RayIntersectsCubeParam("+z", { 0.5, 0, 5 }, { 0, 0, -1 }, 4, 6),
		RayIntersectsCubeParam("-z", { 0.5, 0, -5 }, { 0, 0, 1 }, 4, 6),
		RayIntersectsCubeParam("inside", { 0, 0.5, 0 }, { 0, 0, 1 }, -1, 1)
	)
);
