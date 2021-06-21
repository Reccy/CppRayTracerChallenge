#include "pattern.h"

using CppRayTracerChallenge::Core::Renderer::Pattern;
using CppRayTracerChallenge::Core::Math::Transform;

void Pattern::transform(Transform transform)
{
	m_transform = transform;
}

const Transform Pattern::transform() const
{
	return m_transform;
}
