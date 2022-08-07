#include "pattern.h"

using CppRayTracerChallenge::Core::Renderer::Pattern;
using RML::Transform;

void Pattern::transform(Transform transform)
{
	m_transform = transform;
}

const Transform Pattern::transform() const
{
	return m_transform;
}
