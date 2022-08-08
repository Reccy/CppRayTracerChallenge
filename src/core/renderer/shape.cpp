#include "shape.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	Shape::Shape(std::shared_ptr<Math::IShape> shape) : m_shape(shape) {};

	Shape::Shape(std::shared_ptr<Math::IShape> shape, Material material) : m_shape(shape), m_material(material) {};

	const Material Shape::material() const
	{
		return m_material;
	}

	void Shape::material(const Material material)
	{
		m_material = material;
	};

	Graphics::Color Shape::colorAt(RML::Point worldPosition) const
	{
		RML::Point objectLocalPosition = worldToObject(worldPosition);
		RML::Point patternLocalPosition = m_material.pattern->transform().invert() * objectLocalPosition;

		return m_material.pattern->colorAt(patternLocalPosition);
	}

	std::weak_ptr<IGroup> Shape::parent() const
	{
		return m_parent;
	}

	std::shared_ptr<Math::IShape> Shape::shape() const
	{
		return m_shape;
	}

	void Shape::parent(std::weak_ptr<IGroup> parent)
	{
		m_parent = parent;
	}

	const RML::Point Shape::worldToObject(RML::Point worldPosition) const
	{
		RML::Point result = worldPosition;

		if (!m_parent.expired())
		{
			result = m_parent.lock()->worldToObject(worldPosition);
		}

		return m_shape->transform().invert() * result;
	}

	const RML::Vector Shape::normalToWorld(RML::Vector objectNormal) const
	{
		RML::Vector normal = m_shape->transform().invert().transpose() * objectNormal;
		normal = normal.normalize();

		if (!m_parent.expired())
		{
			normal = m_parent.lock()->normalToWorld(normal);
		}

		return normal;
	}

	void Shape::transform(Transform transform)
	{
		m_shape->transform(transform);
	}

	Transform& Shape::transform()
	{
		return m_shape->transform();
	}

	const RML::Vector Shape::normal(const RML::Point position) const
	{
		const RML::Point localPosition = worldToObject(position);
		const RML::Vector localNormal = normalLocal(localPosition);
		return normalToWorld(localNormal);
	}

	const RML::Vector Shape::normalLocal(const RML::Point position) const
	{
		return m_shape->normalLocal(position);
	}

	const Math::Intersections Shape::intersect(Math::Ray ray) const
	{
		// NOTE: Instead of just delegating to Math::Shape::intersect and returning that result,
		// we need to replace the pointer to the IShape inside each Intersection.

		const Math::Intersections originalResult = m_shape->intersect(ray);
		std::vector<Math::Intersection> newIntersections;

		for (int i = 0; i < originalResult.count(); ++i)
		{
			const Math::Intersection& newIntersection = originalResult.at(i);
			Math::Intersection replacement = Math::Intersection(newIntersection.t(), *this, newIntersection.u(), newIntersection.v());

			newIntersections.push_back(replacement);
		}

		return Math::Intersections(newIntersections);
	}

	const Math::Intersections Shape::intersectLocal(Math::Ray ray) const
	{
		// NOTE: Instead of just delegating to Math::Shape::intersect and returning that result,
		// we need to replace the pointer to the IShape inside each Intersection.

		const Math::Intersections originalResult = m_shape->intersectLocal(ray);
		std::vector<Math::Intersection> newIntersections;

		for (int i = 0; i < originalResult.count(); ++i)
		{
			const Math::Intersection& newIntersection = originalResult.at(i);
			Math::Intersection replacement = Math::Intersection(newIntersection.t(), *this, newIntersection.u(), newIntersection.v());

			newIntersections.push_back(replacement);
		}

		return Math::Intersections(newIntersections);
	}

	const Math::BoundingBox Shape::bounds() const
	{
		return m_shape->bounds();
	}

	const Math::BoundingBox Shape::parentSpaceBounds() const
	{
		return m_shape->parentSpaceBounds();
	}

	bool Shape::operator==(const Shape& other) const
	{
		return (m_shape == other.m_shape) && (m_material == other.m_material);
	}

	bool Shape::operator!=(const Shape& other) const
	{
		return !(other == *this);
	}
}
