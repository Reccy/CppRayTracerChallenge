#include "perturbed.h"

using CppRayTracerChallenge::Core::Renderer::Patterns::Perturbed;
using namespace CppRayTracerChallenge::Core;

Perturbed::Perturbed(const std::shared_ptr<Pattern> pattern) : m_pattern(pattern) {};

Graphics::Color Perturbed::colorAt(Math::Point position) const
{
	Math::Point patternLocal = m_pattern->transform().invert() * position;
	return m_pattern->colorAt(patternLocal);
};

bool Perturbed::operator==(const Perturbed& other) const
{
	return *m_pattern == *other.m_pattern;
}
