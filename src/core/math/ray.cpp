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
