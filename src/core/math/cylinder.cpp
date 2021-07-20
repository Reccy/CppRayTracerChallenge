#include "cylinder.h"
#include "comparison.h"

using namespace CppRayTracerChallenge::Core::Math;

Cylinder::Cylinder() : m_minimum(-INFINITY), m_maximum(INFINITY), m_closed(false) {};

Cylinder::Cylinder(double minimum, double maximum) : m_minimum(minimum), m_maximum(maximum), m_closed(false) {};

Cylinder::Cylinder(double minimum, double maximum, bool closed) : m_minimum(minimum), m_maximum(maximum), m_closed(closed) {};

const Vector Cylinder::normalLocal(const Point position) const
{
	double dist = pow(position.x(), 2) + pow(position.z(), 2);

	if (dist < 1.0 && position.y() >= m_maximum - Math::EPSILON)
	{
		return Vector(0, 1, 0);
	}

	if (dist < 1.0 && position.y() <= m_minimum + Math::EPSILON)
	{
		return Vector(0, -1, 0);
	}

	return Vector(position.x(), 0, position.z());
}

const Intersections Cylinder::intersectLocal(Ray ray) const
{
	double a = pow(ray.direction().x(), 2) + pow(ray.direction().z(), 2);

	// Ray is parallel to the Y Axis
	if (Math::Comparison::equal(a, 0.0))
	{
		return Intersections(intersectCaps(ray, {}));
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

	// swap
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

	auto append = intersectCaps(ray, intersections);

	for (int i = 0; i < append.size(); ++i)
	{
		intersections.push_back(append[i]);
	}

	return Intersections(intersections);
}

const BoundingBox Cylinder::bounds() const
{
	return BoundingBox({ -1, m_minimum, -1 }, { 1, m_maximum, 1 });
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

bool Cylinder::closed() const
{
	return m_closed;
}

void Cylinder::closed(bool closed)
{
	m_closed = closed;
}

bool Cylinder::checkCap(Ray ray, double t) const
{
	double x = ray.origin().x() + t * ray.direction().x();
	double z = ray.origin().z() + t * ray.direction().z();

	return (pow(x, 2) + pow(z, 2)) <= 1;
}

std::vector<Intersection> Cylinder::intersectCaps(Ray ray, Intersections intersections) const
{
	std::vector<Intersection> intersectionsList;

	if (!m_closed || Comparison::equal(ray.direction().y(), 0.0))
	{
		return intersectionsList;
	}

	double t = (m_minimum - ray.origin().y()) / ray.direction().y();
	if (checkCap(ray, t))
	{
		intersectionsList.push_back(Intersection(t, *this));
	}

	t = (m_maximum - ray.origin().y()) / ray.direction().y();
	if (checkCap(ray, t))
	{
		intersectionsList.push_back(Intersection(t, *this));
	}

	return intersectionsList;
}
