#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_RING
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_RING

#include "../pattern.h"
#include "../../helpers/polymorphic_comparable.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	class Ring : public Helpers::PolymorphicComparable<Pattern, Ring>
	{
	public:
		/// <summary>
		/// Creates a ring pattern from color A and color B
		/// </summary>
		/// <param name="a">Color A</param>
		/// <param name="b">Color B</param>
		Ring(Graphics::Color a, Graphics::Color b);

		/// <summary>
		/// Returns Color A
		/// </summary>
		/// <returns>Color A</returns>
		Graphics::Color colorA() const;

		/// <summary>
		/// Returns Color B
		/// </summary>
		/// <returns>Color B</returns>
		Graphics::Color colorB() const;

		Graphics::Color colorAt(RML::Point position) const override;

		bool operator==(const Ring& other) const;
	private:
		Graphics::Color m_colorA;
		Graphics::Color m_colorB;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_RING
