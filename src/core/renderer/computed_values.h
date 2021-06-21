#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES

#include "shape.h"
#include "../math/intersection.h"
#include "../math/ray.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	/// <summary>
	/// Represents a collection of values computed from an intersection and a ray
	/// </summary>
	class ComputedValues
	{
	public:
		ComputedValues() = delete;

		/// <summary>
		/// Computes the values of the intersection and the ray
		/// </summary>
		ComputedValues(Math::Intersection intersection, Math::Ray ray);

		/// <summary>
		/// Returns the t value from the intersection
		/// </summary>
		/// <returns>t</returns>
		double t() const;

		/// <summary>
		/// Returns the shape that was intersected
		/// </summary>
		/// <returns>The reference to the shape</returns>
		const Renderer::Shape& shape() const;

		/// <summary>
		/// Returns the material of the shape that was intersected
		/// </summary>
		/// <returns>Material const reference</returns>
		Renderer::Material material() const;

		/// <summary>
		/// Returns the position of the intersection
		/// </summary>
		/// <returns>Where the intersection occurred</returns>
		Math::Point position() const;

		/// <summary>
		/// Returns the position of the intersection translated slightly in front of the normal.
		/// This accounts for floating point imprecision errors.
		/// </summary>
		/// <returns>Where the intersection occurred, translated in the positive normal direction</returns>
		Math::Point overPosition() const;

		/// <summary>
		/// Returns the eye vector
		/// </summary>
		/// <returns>The eye vector</returns>
		Math::Vector eye() const;

		/// <summary>
		/// Returns the normal vector
		/// </summary>
		/// <returns>The normal vector</returns>
		Math::Vector normal() const;

		/// <summary>
		/// If the intersection occurred inside or outside of the intersectable object
		/// </summary>
		/// <returns>Bool for if intersection was inside or outside</returns>
		bool isInside() const;
	private:
		double m_t;
		Renderer::Shape m_shape;
		Math::Point m_position;
		Math::Point m_overPosition;
		Math::Vector m_eye;
		Math::Vector m_normal;
		bool m_isInside;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES
