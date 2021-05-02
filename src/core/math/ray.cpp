#include "ray.h"

using namespace CppRayTracerChallenge::Core::Math;

Ray::Ray(Point origin, Vector direction) : m_origin(origin), m_direction(direction) {}

Point Ray::origin() const
{
	return m_origin;
}

Vector Ray::direction() const
{
	return m_direction;
}

Point Ray::position(const double t) const
{
	return m_origin + m_direction * t;
}

Intersections Ray::intersect_sphere(const Sphere& sphere) const
{
	Vector sphereToRay = m_origin - sphere.position();
	double a = Vector::dot(m_direction, m_direction);
	double b = 2 * Vector::dot(m_direction, sphereToRay);
	double c = Vector::dot(sphereToRay, sphereToRay) - 1;

	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
	{
		return Intersections();
	}

	double t1 = (-b - sqrt(discriminant)) / 2 * a;
	double t2 = (-b + sqrt(discriminant)) / 2 * a;

	return Intersections({ Intersection(t1, sphere), Intersection(t2, sphere) });
}
