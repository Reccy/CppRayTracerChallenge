#include "test_pattern.h"

CppRayTracerChallenge::Core::Graphics::Color CppRayTracerChallenge::Core::Renderer::Patterns::TestPattern::colorAt(RML::Point position) const
{
	return CppRayTracerChallenge::Core::Graphics::Color(static_cast<float>(position.x()), static_cast<float>(position.y()), static_cast<float>(position.z()));
}

bool CppRayTracerChallenge::Core::Renderer::Patterns::TestPattern::operator==(const CppRayTracerChallenge::Core::Renderer::Patterns::TestPattern&) const
{
	return true;
}