#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_TEST_PATTERN
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_TEST_PATTERN

#include "../../helpers/polymorphic_comparable.h"
#include "../pattern.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	/// <summary>
	/// A test pattern that correlates the position in space with the color
	/// </summary>
	class TestPattern : public Helpers::PolymorphicComparable<Pattern, TestPattern>
	{
	public:
		/// <summary>
		/// Creates the test pattern
		/// </summary>
		TestPattern() = default;

		Graphics::Color colorAt(Math::Point position) const override;

		bool operator==(const TestPattern& other) const;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_TEST_PATTERN
