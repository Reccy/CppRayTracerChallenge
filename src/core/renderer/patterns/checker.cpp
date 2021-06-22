#include "checker.h"

using CppRayTracerChallenge::Core::Renderer::Patterns::Checker;
using namespace CppRayTracerChallenge::Core;

Checker::Checker(Graphics::Color a, Graphics::Color b) : m_colorA(a), m_colorB(b) {};

Graphics::Color Checker::colorA() const
{
	return m_colorA;
}

Graphics::Color Checker::colorB() const
{
	return m_colorB;
}

Graphics::Color Checker::colorAt(Math::Point position) const
{
	if (static_cast<int>(floor(position.x()) + floor(position.y()) + floor(position.z())) % 2 == 0)
	{
		return m_colorA;
	}

	return m_colorB;
}

bool Checker::operator==(const Checker& other) const
{
	return m_colorA == other.m_colorA && m_colorB == other.m_colorB;
}
