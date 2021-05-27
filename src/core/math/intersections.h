#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTIONS
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTIONS

#include <vector>
#include <optional>
#include "intersection.h"
#include "ray.h"

namespace CppRayTracerChallenge::Core::Math
{
	class Ray;
	class Intersection;
	/// <summary>
	/// Represents a sorted collection of Intersections
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
		/// <param name="intersections">A list of intersections, passed as const references. They will be sorted and added to intersections.</param>
		const Intersections(const std::vector<Intersection> intersections);

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

		/// <summary>
		/// Finds the intersection that the ray hits, if it exists.
		/// </summary>
		/// <returns>The intersection that is the hit. Can return null if no hit occurred.</returns>
		const std::optional<const Intersection> hit() const;

		Intersections operator+(const Intersections& other) const;
		Intersections operator+=(const Intersections& other);
		bool operator==(const Intersections& other) const;
		bool operator!=(const Intersections& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Intersections& intersections);
	private:
		std::vector<Intersection> m_intersections;
		int m_hitIndex;

		void calculateHitindex();
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTIONS
