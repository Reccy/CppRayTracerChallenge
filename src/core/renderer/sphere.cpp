#include "sphere.h"
#include "../math/sphere.h"

using namespace CppRayTracerChallenge::Core::Renderer;

Sphere::Sphere() : Math::Sphere(), m_material(Material()) {};

Sphere::Sphere(Material material) : Math::Sphere(), m_material(material) {};

const Material Sphere::material() const
{
	return m_material;
}

void Sphere::material(const Material material)
{
	m_material = material;
};

Math::Intersections Sphere::intersect(const Math::Ray& ray) const
{
	return Math::Intersections::intersect(ray, *this);
}
