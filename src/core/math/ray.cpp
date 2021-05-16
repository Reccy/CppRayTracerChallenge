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

Ray Ray::transform(const Transform& transform) const
{
	Vector direction = transform * m_direction;
	Point origin = transform * m_origin;

	return Ray(origin, direction);
}
