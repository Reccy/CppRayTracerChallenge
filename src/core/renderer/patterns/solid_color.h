#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_SOLIDCOLOR
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_SOLIDCOLOR

#include "../pattern.h"
#include "../../graphics/color.h"
#include "../../math/point.h"
#include "../../helpers/polymorphic_comparable.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	/// <summary>
	/// A solid color
	/// </summary>
	class SolidColor : public Helpers::PolymorphicComparable<Pattern, SolidColor>
	{
	public:
		/// <summary>
		/// Creates the solid color pattern
		/// </summary>
		/// <param name="color">The color to use</param>
		SolidColor(Color color);

		Color colorAt(Math::Point _) const override;

		bool operator==(const SolidColor& other) const;
	private:
		Color m_color;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_SOLIDCOLOR
