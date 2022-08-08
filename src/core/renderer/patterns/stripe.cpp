#include "stripe.h"
#include <cmath>

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	using Graphics::Color;

	Stripe::Stripe(Graphics::Color colorA, Graphics::Color colorB) : m_colorA(colorA), m_colorB(colorB) {};

	Color Stripe::colorA() const
	{
		return m_colorA;
	}

	Color Stripe::colorB() const
	{
		return m_colorB;
	}

	Color Stripe::colorAt(RML::Point position) const
	{
		if (static_cast<int>(floor(position.x() + RML::EPSILON)) % 2 == 0)
		{
			return m_colorA;
		}

		return m_colorB;
	}

	bool Stripe::operator==(const Stripe& other) const
	{
		return m_colorA == other.m_colorA && m_colorB == other.m_colorB;
	}
}
