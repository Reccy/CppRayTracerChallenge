#include "cylinder.h"

using namespace CppRayTracerChallenge::Core::Math;

Cylinder::Cylinder() {};

const Vector Cylinder::normalLocal(const Point) const
{
	return Vector();
}

const Intersections Cylinder::intersectLocal(Ray ray) const
{
	double a = pow(ray.direction().x(), 2) + pow(ray.direction().z(), 2);

	// Ray is parallel to the Y Axis
	if (Math::Comparison::equal(a, 0.0))
	{
		return Intersections();
	}

	double b = 2 * ray.origin().x() * ray.direction().x() +
		2 * ray.origin().z() * ray.direction().z();

	double c = pow(ray.origin().x(), 2) + pow(ray.origin().z(), 2) - 1;

	double disc = pow(b, 2) - 4 * a * c;

	if (disc < 0)
	{
		return Intersections();
	}

	double t0 = (-b - sqrt(disc)) / (2 * a);
	double t1 = (-b + sqrt(disc)) / (2 * a);

	return Intersections({ { t0, *this }, { t1, *this } });
}
