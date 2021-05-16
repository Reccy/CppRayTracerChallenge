#include "object.h"

using namespace CppRayTracerChallenge::Core::Renderer;

Object::Object(Math::Sphere shape) : m_shape(shape), m_material(Material()) {};

Object::Object(Math::Sphere shape, Material material) : m_shape(shape), m_material(material) {};

const Material Object::material() const
{
	return m_material;
}

void Object::material(const Material material)
{
	m_material = material;
};

Math::Intersections Object::intersect(const Math::Ray& ray) const
{
	return m_shape.intersect(ray);
}
