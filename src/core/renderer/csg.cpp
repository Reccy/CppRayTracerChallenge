#include "csg.h"
#include "../math/shape.h"
#include "../math/intersections.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	using Math::Intersection;
	using Math::Intersections;
	using Math::Transform;
	using Math::BoundingBox;
	using RML::Vector;
	using RML::Point;

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

	void CSG::transform(Transform transform)
	{
		m_transform = transform;
	}

	Transform& CSG::transform()
	{
		return m_transform;
	}

	const Vector CSG::normal(const RML::Point position) const
	{
		(void)position;
		throw std::logic_error("Method not implemented");
	}

	const Vector CSG::normalLocal(const RML::Point position) const
	{
		(void)position;
		throw std::logic_error("Method not implemented");
	}

	const Intersections CSG::intersect(Math::Ray ray) const
	{
		ray = ray.transform(m_transform.invert().matrix());

		return intersectLocal(ray);
	}

	const Intersections CSG::intersectLocal(Math::Ray ray) const
	{
		auto leftIntersections = m_left->intersect(ray);
		auto rightIntersections = m_right->intersect(ray);

		auto result = leftIntersections + rightIntersections;

		return filterIntersections(result);
	}

	const BoundingBox CSG::bounds() const
	{
		BoundingBox result = BoundingBox();

		result.add(m_left->parentSpaceBounds());
		result.add(m_right->parentSpaceBounds());

		return result;
	}

	const BoundingBox CSG::parentSpaceBounds() const
	{
		BoundingBox result = BoundingBox(bounds());
		result.transform(m_transform);
		return result;
	}

	const Point CSG::worldToObject(Point worldPosition) const
	{
		(void)worldPosition;
		throw std::logic_error("Method not implemented");
	};

	const Vector CSG::normalToWorld(Vector objectNormal) const
	{
		(void)objectNormal;
		throw std::logic_error("Method not implemented");
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

}
