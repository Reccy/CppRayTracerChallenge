#include "plane.h"

namespace CppRayTracerChallenge::Core::Math
{
	using RML::INF;
	using RML::EPSILON;

	Plane::Plane() : Shape() {};

	const Vector Plane::normalLocal(const Point) const
	{
		return Vector::up();
	}

	const Intersections Plane::intersectLocal(const Ray ray) const
	{
		// If the ray is parallel to the plane
		if (abs(ray.direction().y()) < EPSILON)
		{
			return Intersections();
		}

		double t = -ray.origin().y() / ray.direction().y();

		return Intersections({ Intersection(t, *this) });
	}

	const BoundingBox Plane::bounds() const
	{
		return BoundingBox({ -INF, 0, -INF }, { INF, 0, INF });
	}

}