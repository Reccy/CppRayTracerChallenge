#include "sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

Sphere::Sphere() {};

const Vector Sphere::normal(const Point position) const
{
	Point objectPoint = m_transform.invert() * position;
	Vector objectNormal = objectPoint - m_position;
	Vector worldNormal = m_transform.invert().transpose() * objectNormal;
	return worldNormal.normalize();
}
