#include <cmath>
#include "pattern.h"

using namespace CppRayTracerChallenge::Core::Renderer;

Pattern::Pattern(Color a, Color b) : m_colorA(a), m_colorB(b) {};

Color Pattern::colorA() const
{
	return m_colorA;
}

Color Pattern::colorB() const
{
	return m_colorB;
}

Color Pattern::colorAt(const Math::Point position) const
{
	if (static_cast<int>(floor(position.x())) % 2 == 0)
	{
		return m_colorA;
	}

	return m_colorB;
}
