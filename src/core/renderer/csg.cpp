#include "csg.h"
#include "../math/shape.h"

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

bool CSG::intersectionAllowed(Operation op, bool lhit, bool inl, bool inr)
{
	if (op == Operation::UNION)
	{
		return (lhit && !inr) || (!lhit && !inl);
	}

	if (op == Operation::INTERSECT)
	{
		return (lhit && inr) || (!lhit && inl);
	}

	if (op == Operation::DIFFERENCE)
	{
		return (lhit && !inr) || (!lhit && inl);
	}

	return false;
}

Intersections CSG::filterIntersections(Intersections intersections) const
{
	bool inl = false;
	bool inr = false;

	Intersections result = Intersections();

	for (int i = 0; i < intersections.count(); ++i)
	{
		const Intersection& intersection = intersections.at(i);

		bool lhit = false;

		if (auto leftGroup = dynamic_cast<IGroup*>(m_left.get()))
		{
			lhit = leftGroup->includes(intersection.shape());
		}
		else if (auto leftShape = dynamic_cast<Shape*>(m_left.get()))
		{
			lhit = leftShape == &intersection.shape();
		}

		if (intersectionAllowed(m_operation, lhit, inl, inr))
		{
			result += intersection;
		}

		if (lhit)
		{
			inl = !inl;
		}
		else
		{
			inr = !inr;
		}
	}

	return result;
}

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

bool CSG::includes(const IShape& other) const
{
	if (auto csg = dynamic_cast<const Renderer::CSG*>(&other))
	{
		return csg->includes(other);
	}
	else if (auto rShape = dynamic_cast<const Renderer::Shape*>(&other))
	{
		return m_left.get() == rShape || m_right.get() == rShape;
	}
	else if (auto mShape = dynamic_cast<const Math::Shape*>(&other))
	{
		return m_left->shape().get() == mShape || m_right->shape().get() == mShape;
	}

	return false;
}

CSG::Operation CSG::operation() const
{
	return m_operation;
}

std::shared_ptr<CppRayTracerChallenge::Core::Renderer::Shape> CSG::left() const
{
	return m_left;
}

std::shared_ptr <CppRayTracerChallenge::Core::Renderer::Shape> CSG::right() const
{
	return m_right;
}
