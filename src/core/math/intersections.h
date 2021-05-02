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
		/// <summary>
		/// Creates an empty collection of Intersections
		/// </summary>
		Intersections();

		/// <summary>
		/// Creates a collection of Intersections
		/// </summary>
		/// <param name="intersections">A list of intersections, passed as const references</param>
		Intersections(const std::vector<Intersection> intersections);

		/// <summary>
		/// Returns a const reference to the intersection at the index
		/// </summary>
		/// <param name="index">The index of the intersection</param>
		/// <returns>Returns a const reference to the intersection at the index</returns>
		const Intersection& at(unsigned int index) const;

		/// <summary>
		/// Returns how many intersections are in the collection
		/// </summary>
		/// <returns>How many intersections are in the collection</returns>
		const int size() const;

		/// <summary>
		/// Returns how many intersections are in the collection.
		/// Alias of size();
		/// </summary>
		/// <returns>How many intersections are in the collection</returns>
		const int count() const;
	private:
		const std::vector<Intersection> m_intersections;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTIONS
