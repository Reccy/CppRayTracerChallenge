#include "cube.h"
#include <algorithm>

using namespace CppRayTracerChallenge::Core::Math;

Cube::Cube() {}

struct CheckAxisResult
{
	double tMin;
	double tMax;
};

CheckAxisResult checkAxis(double origin, double direction)
{
	double tMinNumerator = (-1 - origin);
	double tMaxNumerator = (1 - origin);

	double tMin, tMax;

	if (abs(direction) >= EPSILON)
	{
		tMin = tMinNumerator / direction;
		tMax = tMaxNumerator / direction;
	}
	else
	{
		tMin = tMinNumerator * INFINITY;
		tMax = tMaxNumerator * INFINITY;
	}

	CheckAxisResult result;

	if (tMin > tMax)
	{
		result.tMin = tMax;
		result.tMax = tMin;
	}
	else
	{
		result.tMin = tMin;
		result.tMax = tMax;
	}

	return result;
}

const Vector Cube::normalLocal(const Point) const
{
	return Vector();
}

const Intersections Cube::intersectLocal(Ray ray) const
{
	auto x = checkAxis(ray.origin().x(), ray.direction().x());
	auto y = checkAxis(ray.origin().y(), ray.direction().y());
	auto z = checkAxis(ray.origin().z(), ray.direction().z());

	double tMin = std::max({ x.tMin, y.tMin, z.tMin });
	double tMax = std::min({ x.tMax, y.tMax, z.tMax });

	std::vector<Intersection> xs{ {tMin, *this}, {tMax, *this} };
	return Intersections(xs);
}
