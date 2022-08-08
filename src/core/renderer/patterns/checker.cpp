#include "checker.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	Checker::Checker(Graphics::Color a, Graphics::Color b) : m_colorA(a), m_colorB(b) {};

	Graphics::Color Checker::colorA() const
	{
		return m_colorA;
	}

	Graphics::Color Checker::colorB() const
	{
		return m_colorB;
	}

	Graphics::Color Checker::colorAt(RML::Point position) const
	{
		if (static_cast<int>(floor(position.x() + RML::EPSILON) + floor(position.y() + RML::EPSILON) + floor(position.z() + RML::EPSILON)) % 2 == 0)
		{
			return m_colorA;
		}

		return m_colorB;
	}

	bool Checker::operator==(const Checker& other) const
	{
		return m_colorA == other.m_colorA && m_colorB == other.m_colorB;
	}
}
