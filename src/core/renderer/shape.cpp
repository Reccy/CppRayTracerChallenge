#include "shape.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

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

Graphics::Color Shape::colorAt(Math::Point worldPosition) const
{
	Math::Point objectLocalPosition = m_shape->transform().invert() * worldToObject(worldPosition);
	Math::Point patternLocalPosition = m_material.pattern->transform().invert() * objectLocalPosition;

	return m_material.pattern->colorAt(patternLocalPosition);
}

std::weak_ptr<Group> Shape::parent() const
{
	return m_parent;
}

void Shape::parent(std::weak_ptr<Group> parent)
{
	m_parent = parent;
}

const Math::Point Shape::worldToObject(Math::Point worldPosition) const
{
	Math::Point result = worldPosition;

	if (!m_parent.expired())
	{
		result = m_parent.lock()->worldToObject(worldPosition);
	}

	return m_shape->transform().invert() * result;
}

const Math::Vector Shape::normalToWorld(Math::Vector objectNormal) const
{
	Math::Vector normal = m_shape->transform().invert().transpose() * objectNormal;
	normal = normal.normalize();

	if (!m_parent.expired())
	{
		normal = m_parent.lock()->normalToWorld(normal);
	}

	return normal;
}

void Shape::transform(Math::Transform transform)
{
	m_shape->transform(transform);
}

const Math::Transform Shape::transform() const
{
	return m_shape->transform();
}

const Math::Vector Shape::normal(const Math::Point position) const
{
	const Math::Point localPosition = worldToObject(position);
	const Math::Vector localNormal = normalLocal(localPosition);
	return normalToWorld(localNormal);
}

const Math::Vector Shape::normalLocal(const Math::Point position) const
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
		Math::Intersection replacement = Math::Intersection(newIntersection.t(), *this);

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
		Math::Intersection replacement = Math::Intersection(newIntersection.t(), *this);

		newIntersections.push_back(replacement);
	}

	return Math::Intersections(newIntersections);
}

bool Shape::operator==(const Shape& other) const
{
	return (m_shape == other.m_shape) && (m_material == other.m_material);
}

bool Shape::operator!=(const Shape& other) const
{
	return !(other == *this);
}
