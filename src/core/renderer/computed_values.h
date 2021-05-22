#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES

#include "../math/intersection.h"

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
		const Math::IIntersectable& intersectable() const;

		/// <summary>
		/// Returns the position of the intersection
		/// </summary>
		/// <returns>Where the intersection occurred</returns>
		Math::Point position() const;

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
		const Math::IIntersectable& m_intersectable;
		Math::Point m_position;
		Math::Vector m_eye;
		Math::Vector m_normal;
		bool m_isInside;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_COMPUTED_VALUES
