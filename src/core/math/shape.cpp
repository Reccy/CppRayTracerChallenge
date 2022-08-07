#include "shape.h"

using namespace CppRayTracerChallenge::Core::Math;

void Shape::transform(const RML::Transform transform)
{
	m_transform = transform;
}

RML::Transform& Shape::transform()
{
	return m_transform;
}

const Vector Shape::normal(const RML::Point position) const
{
	auto ti1 = m_transform.get_inverted();
	RML::Point localPoint = ti1 * position;
	RML::Vector localNormal = normalLocal(localPoint);

	auto ti2 = m_transform.get_transposed();
	auto ti3 = ti2.invert();
	RML::Vector worldNormal = ti3 * localNormal;
	return worldNormal.normalize();
}

const Intersections Shape::intersect(Ray ray) const
{
	ray = ray.transform(m_transform.get_inverted());

	return intersectLocal(ray);
}

const BoundingBox Shape::parentSpaceBounds() const
{
	BoundingBox result = BoundingBox(bounds());
	result.transform(m_transform);
	return result;
}
