#include "cube.h"
#include <algorithm>
#include "comparison.h"

namespace CppRayTracerChallenge::Core::Math
{
	using RML::INF;
	using RML::EPSILON;

	Cube::Cube() {}

	Cube::CheckAxisResult Cube::checkAxis(double origin, double direction) const
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

	const Vector Cube::normalLocal(const Point position) const
	{
		double maxComponent = std::max({ abs(position.x()), abs(position.y()), abs(position.z()) });

		if (Math::Comparison::equal(maxComponent, abs(position.x())))
		{
			return Vector(position.x(), 0, 0);
		}
		else if (Math::Comparison::equal(maxComponent, abs(position.y())))
		{
			return Vector(0, position.y(), 0);
		}

		return Vector(0, 0, position.z());
	}

	const Intersections Cube::intersectLocal(Ray ray) const
	{
		auto x = checkAxis(ray.origin().x(), ray.direction().x());
		auto y = checkAxis(ray.origin().y(), ray.direction().y());
		auto z = checkAxis(ray.origin().z(), ray.direction().z());

		double tMin = std::max({ x.tMin, y.tMin, z.tMin });
		double tMax = std::min({ x.tMax, y.tMax, z.tMax });

		if (tMin > tMax)
		{
			return Intersections();
		}

		if (tMax < 0)
		{
			return Intersections();
		}

		std::vector<Intersection> xs{ {tMin, *this}, {tMax, *this} };
		return Intersections(xs);
	}

	const BoundingBox Cube::bounds() const
	{
		return BoundingBox({ -1,-1,-1 }, { 1,1,1 });
	}
}
