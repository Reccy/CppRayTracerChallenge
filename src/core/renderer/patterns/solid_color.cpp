#include "solid_color.h"

using namespace CppRayTracerChallenge::Core::Renderer::Patterns;
using namespace CppRayTracerChallenge::Core::Graphics;

SolidColor::SolidColor(Color color) : m_color(color) {}

Color SolidColor::color() const
{
	return m_color;
}

#pragma warning(disable: 4100)
Color SolidColor::colorAt(Math::Point _) const
{
	return m_color;
}

bool SolidColor::operator==(const SolidColor& other) const
{
	return m_color == other.m_color;
}
