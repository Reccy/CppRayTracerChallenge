#include "perturbed.h"

using CppRayTracerChallenge::Core::Renderer::Patterns::Perturbed;
using namespace CppRayTracerChallenge::Core;

Perturbed::Perturbed(const std::shared_ptr<Pattern> pattern) : m_pattern(pattern) {};

Graphics::Color Perturbed::colorAt(Math::Point position) const
{
	return m_pattern->colorAt(position);
};

bool Perturbed::operator==(const Perturbed& other) const
{
	return *m_pattern == *other.m_pattern;
}
