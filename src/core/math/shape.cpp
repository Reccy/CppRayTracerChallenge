#include "shape.h"

using namespace CppRayTracerChallenge::Core::Math;

void Shape::transform(const Transform transform)
{
	m_transform = transform;
}

const Transform Shape::transform() const
{
	return m_transform;
}

const Vector Shape::normal(const Point position) const
{
	Point localPoint = m_transform.invert() * position;
	Vector localNormal = normalLocal(localPoint);
	Vector worldNormal = m_transform.invert().transpose() * localNormal;
	return worldNormal.normalize();
}

const Intersections Shape::intersect(Ray ray) const
{
	ray = ray.transform(m_transform.invert());

	return intersectLocal(ray);
}
