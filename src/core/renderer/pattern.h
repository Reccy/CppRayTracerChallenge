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
		virtual ~Pattern() {};

		/// <summary>
		/// Returns the color of the pattern at a specific point in local space
		/// </summary>
		/// <param name="position">The position in local space</param>
		/// <returns>The pattern color at the position in local space</returns>
		virtual Color colorAt(const Math::Point position) const = 0;

		virtual bool operator==(const Pattern& other) const = 0;
		bool operator!=(const Pattern& other) const
		{
			return !(*this == other);
		};
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERN
