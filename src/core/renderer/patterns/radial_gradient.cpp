#include "radial_gradient.h"
#include <cmath>

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	RadialGradient::RadialGradient(Graphics::Color a, Graphics::Color b) : m_colorA(a), m_colorB(b) {};

	Graphics::Color RadialGradient::colorA() const
	{
		return m_colorA;
	}

	Graphics::Color RadialGradient::colorB() const
	{
		return m_colorB;
	}

	Graphics::Color RadialGradient::colorAt(RML::Point position) const
	{
		RML::Point realPos = RML::Point(position.x(), 0, position.z());

		double distance = (realPos - RML::Point(0, 0, 0)).magnitude();
		float fraction = static_cast<float>(std::fmod(distance, 1));

		return Graphics::Color::blend(m_colorA, m_colorB, fraction);
	}

	bool RadialGradient::operator==(const RadialGradient& other) const
	{
		return m_colorA == other.m_colorA && m_colorB == other.m_colorB;
	}
}
