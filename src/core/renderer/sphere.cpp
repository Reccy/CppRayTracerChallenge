#include "sphere.h"

using namespace CppRayTracerChallenge::Core::Renderer;

Sphere::Sphere() : m_material(Material()) {};

const Material Sphere::material() const
{
	return m_material;
}

void Sphere::material(const Material material)
{
	m_material = material;
}
