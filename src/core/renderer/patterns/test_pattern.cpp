#include "test_pattern.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{

	Graphics::Color Patterns::TestPattern::colorAt(RML::Point position) const
	{
		return Graphics::Color(static_cast<float>(position.x()), static_cast<float>(position.y()), static_cast<float>(position.z()));
	}

	bool TestPattern::operator==(const TestPattern&) const
	{
		return true;
	}
}
