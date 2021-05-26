#include "shape.h"

using namespace CppRayTracerChallenge::Core::Renderer;

Shape::Shape(Math::IShape& shape) : m_shape(shape) {};

Shape::Shape(Math::IShape& shape, Material material) : m_shape(shape), m_material(material) {};

void Shape::transform(Math::Transform transform)
{
	m_shape.transform(transform);
}

const Math::Transform Shape::transform() const
{
	return m_shape.transform();
}

const Math::Vector Shape::normal(const Math::Point position) const
{
	return m_shape.normal(position);
}

const Material Shape::material() const
{
	return m_material;
}

void Shape::material(const Material material)
{
	m_material = material;
};
