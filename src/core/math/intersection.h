#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION

#include "i_intersectable.h"

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
		Intersection(const double t, const IIntersectable& intersectable);
	private:
		const double m_t;
		const IIntersectable& m_intersectable;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION
