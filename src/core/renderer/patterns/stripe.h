#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_STRIPE
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_STRIPE

#include "../pattern.h"
#include "../../math/point.h"
#include "../../helpers/polymorphic_comparable.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	/// <summary>
	/// A color pattern that alternates between color A and color B
	/// </summary>
	class Stripe : public Helpers::PolymorphicComparable<Pattern, Stripe>
	{
	public:
		/// <summary>
		/// Creates the stripe with 2 colors
		/// </summary>
		/// <param name="a">Color A</param>
		/// <param name="b">Color B</param>
		Stripe(Graphics::Color a, Graphics::Color b);

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

		Graphics::Color colorAt(Math::Point position) const override;

		bool operator==(const Stripe& other) const;
	private:
		Graphics::Color m_colorA;
		Graphics::Color m_colorB;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_STRIPE
