#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION

#include "sphere.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// Represents an intersection on an object from a ray and the time from the ray as 't'
	/// </summary>
	class Intersection
	{
	public:
		Intersection() = delete;

		/// <summary>
		/// Creates a new intersection
		/// </summary>
		/// <param name="t">The distrance from the ray origin in which to calculate the new position</param>
		/// <param name="intersectable">An object that can be intersected by the ray</param>
		Intersection(const double t, const Sphere& intersectable); // NOTE: Temporarily removing IIntersectable. Replace with Shape in future refactor

		/// <summary>
		/// Returns the stored value t
		/// </summary>
		/// <returns>Returns the stored value t</returns>
		const double t() const;

		/// <summary>
		/// Returns a const reference to the intersectable object
		/// </summary>
		/// <returns>Returns the stored intersectable</returns>
		const Sphere& intersectable() const; // NOTE: Temporarily removing IIntersectable. Replace with Shape in future refactor

		bool operator==(const Intersection& other) const;
		bool operator!=(const Intersection& other) const;
	private:
		const double m_t;
		const Sphere& m_intersectable; // NOTE: Temporarily removing IIntersectable. Replace with Shape in future refactor
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION
