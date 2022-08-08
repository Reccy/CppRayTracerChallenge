#include "solid_color.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	using Graphics::Color;

	SolidColor::SolidColor(Color color) : m_color(color) {}

	Color SolidColor::color() const
	{
		return m_color;
	}

	Color SolidColor::colorAt(RML::Point) const
	{
		return m_color;
	}

	bool SolidColor::operator==(const SolidColor& other) const
	{
		return m_color == other.m_color;
	}
}
