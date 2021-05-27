#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERN
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERN

#include "../graphics/color.h"
#include "../math/point.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	using namespace CppRayTracerChallenge::Core::Graphics;

	/// <summary>
	/// Represents a color pattern that can be used to vary the color of materials
	/// </summary>
	class Pattern
	{
	public:
		Pattern() = delete;

		/// <summary>
		/// Creates the pattern with 2 colors
		/// </summary>
		/// <param name="a">Color A</param>
		/// <param name="b">Color B</param>
		Pattern(Color a, Color b);

		/// <summary>
		/// Returns Color A
		/// </summary>
		/// <returns>Color A</returns>
		Color colorA() const;

		/// <summary>
		/// Returns Color B
		/// </summary>
		/// <returns>Color B</returns>
		Color colorB() const;

		/// <summary>
		/// Returns the color of the pattern at a specific point in local space
		/// </summary>
		/// <param name="position">The position in local space</param>
		/// <returns>The pattern color at the position in local space</returns>
		Color colorAt(const Math::Point position) const;
	private:
		const Color m_colorA;
		const Color m_colorB;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERN
