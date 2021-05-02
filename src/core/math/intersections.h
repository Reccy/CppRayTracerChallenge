#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTIONS
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTIONS

#include <vector>
#include "intersection.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// Represents a collection of Intersections
	/// </summary>
	class Intersections
	{
	public:
		Intersections() = delete;

		/// <summary>
		/// Creates a collection of Intersections
		/// </summary>
		/// <param name="intersections">A list of intersections, passed as const references</param>
		Intersections(const std::vector<std::reference_wrapper<const Intersection>> intersections);

		/// <summary>
		/// Returns how many intersections are in the collection
		/// </summary>
		/// <returns>How many intersections are in the collection</returns>
		const int count() const;
	private:
		const std::vector<std::reference_wrapper<const Intersection>> m_intersections;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTIONS
