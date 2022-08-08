#pragma once
#include "../graphics/color.h"
#include "../math/transform.h"

namespace CppRayTracerChallenge::Core::Renderer
{
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
		virtual ::CppRayTracerChallenge::Core::Graphics::Color colorAt(const RML::Point position) const = 0;

		/// <summary>
		/// Transforms the pattern
		/// </summary>
		/// <param name="transform">The transform to apply to the pattern</param>
		void transform(Math::Transform transform);

		/// <summary>
		/// Returns a copy of the pattern's transform
		/// </summary>
		/// <returns></returns>
		const Math::Transform transform() const;

		virtual bool operator==(const Pattern& other) const = 0;

		bool operator!=(const Pattern& other) const
		{
			return !(*this == other);
		};
	protected:
		Math::Transform m_transform;
	};
}
