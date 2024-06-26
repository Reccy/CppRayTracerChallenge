#pragma once
#include "RML.h"
#include "../graphics/color.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	/// <summary>
	/// Represents a point in space that emits light
	/// </summary>
	class PointLight
	{
	public:
		PointLight() = delete;

		/// <summary>
		/// Creates a new point light at a position with the set intensity
		/// </summary>
		/// <param name="position">The position to place the Point Light</param>
		/// <param name="intensity">The intensity and color of the light</param>
		PointLight(RML::Point position, Graphics::Color intensity);

		/// <summary>
		/// Returns the position of the light
		/// </summary>
		/// <returns>Returns the position of the light</returns>
		const RML::Point position() const;

		/// <summary>
		/// Returns the intensity / color of the light
		/// </summary>
		/// <returns>Returns the intensity / color of the light</returns>
		const Graphics::Color intensity() const;
	private:
		RML::Point m_position;
		Graphics::Color m_intensity;
	};
}
