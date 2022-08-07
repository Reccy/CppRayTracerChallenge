#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_GRADIENT
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_GRADIENT

#include "../pattern.h"
#include "RML.h"
#include "../../helpers/polymorphic_comparable.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	class Gradient : public Helpers::PolymorphicComparable<Pattern, Gradient>
	{
	public:
		/// <summary>
		/// Creates a gradient from color A to color B
		/// </summary>
		/// <param name="a">Color A</param>
		/// <param name="b">Color B</param>
		Gradient(Graphics::Color a, Graphics::Color b);

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

		bool operator==(const Gradient& other) const;
	private:
		Graphics::Color m_colorA;
		Graphics::Color m_colorB;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_GRADIENT
