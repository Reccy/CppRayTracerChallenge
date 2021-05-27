#include "shape.h"

using namespace CppRayTracerChallenge::Core::Renderer;

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
	return m_shape->normal(position);
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
