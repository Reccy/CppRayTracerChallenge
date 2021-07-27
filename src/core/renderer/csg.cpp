#include "csg.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

std::shared_ptr<CSG> CSG::build(Operation op, std::shared_ptr<Shape> left, std::shared_ptr<Shape> right)
{
	CSG csg = CSG(op);
	auto result = std::make_shared<CSG>(csg);
	result->m_left = left;
	result->m_right = right;

	auto ptr = result->weak_from_this();
	left->parent(ptr);
	right->parent(ptr);

	return result;
}

CSG::CSG(Operation op)
	: m_operation(op) {};

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
	// TODO
	return position;
}

const Vector CSG::normalLocal(const Math::Point position) const
{
	// TODO
	return position;
}

const Intersections CSG::intersect(Math::Ray) const
{
	// TODO
	return Math::Intersections();
}

const Intersections CSG::intersectLocal(Math::Ray) const
{
	// TODO
	return Math::Intersections();
}

const BoundingBox CSG::bounds() const
{
	// TODO
	return Math::BoundingBox();
}

const BoundingBox CSG::parentSpaceBounds() const
{
	// TODO
	return Math::BoundingBox();
}

const Point CSG::worldToObject(Point worldPosition) const
{
	// TODO
	return worldPosition;
};

const Vector CSG::normalToWorld(Vector objectNormal) const
{
	// TODO
	return objectNormal;
};

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
