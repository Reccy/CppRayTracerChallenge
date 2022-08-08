#include "pattern.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	using Math::Transform;

	void Pattern::transform(Transform transform)
	{
		m_transform = transform;
	}

	const Transform Pattern::transform() const
	{
		return m_transform;
	}
}
