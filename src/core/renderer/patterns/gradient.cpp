#include "gradient.h"

using CppRayTracerChallenge::Core::Renderer::Patterns::Gradient;
using CppRayTracerChallenge::Core::Graphics::Color;
using CppRayTracerChallenge::Core::Math::Point;

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
	Color difference = m_colorB - m_colorA;
	float fraction = static_cast<float>(localPosition.x()) - floor(static_cast<float>(localPosition.x()));

	return m_colorA + difference * fraction;
}

bool Gradient::operator==(const Gradient& other) const
{
	return m_colorA == other.m_colorA && m_colorB == other.m_colorB;
}
