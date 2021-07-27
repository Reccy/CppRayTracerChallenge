#include "csg.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

CSG::CSG(Operation op, std::shared_ptr<Shape> left, std::shared_ptr<Shape> right)
	: m_operation(op), m_left(left), m_right(right) {};

void CSG::transform(Math::Transform transform)
{
	m_transform = transform;
}

const Transform CSG::transform() const
{
	return m_transform;
}

const Vector CSG::normal(const Math::Point position) const
{
	return position;
}

const Vector CSG::normalLocal(const Math::Point position) const
{
	return position;
}

const Intersections CSG::intersect(Math::Ray) const
{
	return Math::Intersections();
}

const Intersections CSG::intersectLocal(Math::Ray) const
{
	return Math::Intersections();
}

const BoundingBox CSG::bounds() const
{
	return Math::BoundingBox();
}

const BoundingBox CSG::parentSpaceBounds() const
{
	return Math::BoundingBox();
}

CSG::Operation CSG::operation() const
{
	return m_operation;
}

std::shared_ptr<Shape> CSG::left() const
{
	return m_left;
}

std::shared_ptr<Shape> CSG::right() const
{
	return m_right;
}
