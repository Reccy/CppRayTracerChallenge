#include "gradient.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	using RML::Point;
	using Graphics::Color;

	Gradient::Gradient(Color colorA, Color colorB) : m_colorA(colorA), m_colorB(colorB) {};

	Color Gradient::colorA() const
	{
		return m_colorA;
	}

	Color Gradient::colorB() const
	{
		return m_colorB;
	}

	Color Gradient::colorAt(Point localPosition) const
	{
		float fraction = static_cast<float>(localPosition.x()) - floor(static_cast<float>(localPosition.x()));

		return Color::blend(m_colorA, m_colorB, fraction);
	}

	bool Gradient::operator==(const Gradient& other) const
	{
		return m_colorA == other.m_colorA && m_colorB == other.m_colorB;
	}

}
