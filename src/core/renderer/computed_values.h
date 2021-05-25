#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES

#include "sphere.h"
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
		/// Returns a const reference to the intersectable
		/// </summary>
		/// <returns>Intersectable const reference</returns>
		const Renderer::Sphere& shape() const;

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
		const Renderer::Sphere& m_intersectable;
		Math::Point m_position;
		Math::Point m_overPosition;
		Math::Vector m_eye;
		Math::Vector m_normal;
		bool m_isInside;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES
