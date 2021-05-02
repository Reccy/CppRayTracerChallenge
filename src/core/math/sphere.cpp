#include "sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

Sphere::Sphere() : m_position(Point(0, 0, 0)) {};

Point Sphere::position() const
{
	return m_position;
}

void Sphere::transform(Transform transform)
{
	m_transform = transform;
}

Transform Sphere::transform() const
{
	return m_transform;
}
