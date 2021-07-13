#include "cylinder.h"

using namespace CppRayTracerChallenge::Core::Math;

Cylinder::Cylinder() : m_minimum(-INFINITY), m_maximum(INFINITY) {};

Cylinder::Cylinder(double minimum, double maximum) : m_minimum(minimum), m_maximum(maximum) {};

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

	if (t0 > t1)
	{
		double tTemp = t0;
		t0 = t1;
		t1 = tTemp;
	}

	std::vector<Intersection> intersections;

	double y0 = ray.origin().y() + t0 * ray.direction().y();
	if (m_minimum < y0 && y0 < m_maximum)
	{
		intersections.push_back(Intersection(t0, *this));
	}

	double y1 = ray.origin().y() + t1 * ray.direction().y();
	if (m_minimum < y1 && y1 < m_maximum)
	{
		intersections.push_back(Intersection(t1, *this));
	}

	return Intersections(intersections);
}

double Cylinder::maximum() const
{
	return m_maximum;
}

void Cylinder::maximum(double maximum)
{
	m_maximum = maximum;
}

double Cylinder::minimum() const
{
	return m_minimum;
}

void Cylinder::minimum(double minimum)
{
	m_minimum = minimum;
}
