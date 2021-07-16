#include "group.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

void Group::transform(Transform transform)
{
	m_transform = transform;
}

const Transform Group::transform() const
{
	return m_transform;
}

const Vector Group::normal(const Point) const
{
	return Vector();
}

const Vector Group::normalLocal(const Point) const
{
	return Vector();
}

const Intersections Group::intersect(Ray) const
{
	return Intersections();
}

const Intersections Group::intersectLocal(Ray) const
{
	return Intersections();
}

int Group::size() const
{
	return static_cast<int>(m_shapes.size());
}

int Group::count() const
{
	return size();
}

bool Group::empty() const
{
	return size() == 0;
}
