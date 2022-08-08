#pragma once
#include "../pattern.h"
#include "../../graphics/color.h"
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
		SolidColor(CppRayTracerChallenge::Core::Graphics::Color color);

		/// <summary>
		/// Returns the color of the pattern
		/// </summary>
		/// <returns></returns>
		CppRayTracerChallenge::Core::Graphics::Color color() const;

		CppRayTracerChallenge::Core::Graphics::Color colorAt(RML::Point _) const override;

		bool operator==(const SolidColor& other) const;
	private:
		CppRayTracerChallenge::Core::Graphics::Color m_color;
	};
}
