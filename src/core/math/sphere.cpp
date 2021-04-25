#include "sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

Sphere::Sphere() : m_position(Point(0, 0, 0)), m_radius(1) {};

Point Sphere::position() const
{
	return m_position;
}

double Sphere::radius() const
{
	return m_radius;
}
