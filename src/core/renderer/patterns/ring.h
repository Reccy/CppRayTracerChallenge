#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_RING
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_RING

#include "../pattern.h"
#include "../../helpers/polymorphic_comparable.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	class Ring : public Helpers::PolymorphicComparable<Pattern, Ring>
	{
	public:
		Ring(Graphics::Color a, Graphics::Color b);

		Graphics::Color colorA() const;

		Graphics::Color colorB() const;

		Graphics::Color colorAt(Math::Point position) const override;

		bool operator==(const Ring& other) const;
	private:
		Graphics::Color m_colorA;
		Graphics::Color m_colorB;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_RING
