#include "cylinder.h"

using namespace CppRayTracerChallenge::Core::Math;

Cylinder::Cylinder() : m_minimum(-INFINITY), m_maximum(INFINITY) {};

const Vector Cylinder::normalLocal(const Point position) const
{
	return Vector(position.x(), 0, position.z());
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

double Cylinder::maximum() const
{
	return m_maximum;
}

double Cylinder::minimum() const
{
	return m_minimum;
}
