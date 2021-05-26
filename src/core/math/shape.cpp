#include "shape.h"

using namespace CppRayTracerChallenge::Core::Math;

void Shape::transform(const Transform transform)
{
	m_transform = transform;
}

const Transform Shape::transform() const
{
	return m_transform;
}
