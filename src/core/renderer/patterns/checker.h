#pragma once
#include "RML.h"
#include "../pattern.h"
#include "../../helpers/polymorphic_comparable.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	class Checker : public Helpers::PolymorphicComparable<Pattern, Checker>
	{
	public:
		/// <summary>
		/// Creates a 3D Checker pattern with Color A and Color B
		/// </summary>
		/// <param name="a">Color A</param>
		/// <param name="b">Color B</param>
		Checker(Graphics::Color a, Graphics::Color b);

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

		bool operator==(const Checker& other) const;
	private:
		Graphics::Color m_colorA;
		Graphics::Color m_colorB;
	};
}
