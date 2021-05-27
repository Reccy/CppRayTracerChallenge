#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION

#include "i_shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	class IShape;
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
		/// <param name="shape">An object that can be intersected by the ray</param>
		Intersection(const double t, const IShape& shape);

		/// <summary>
		/// Returns the stored value t
		/// </summary>
		/// <returns>Returns the stored value t</returns>
		const double t() const;

		/// <summary>
		/// Returns a const reference to the intersectable object
		/// </summary>
		/// <returns>Returns the stored intersectable</returns>
		const IShape& shape() const;

		bool operator==(const Intersection& other) const;
		bool operator!=(const Intersection& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Intersection& intersection);
	private:
		const double m_t;
		const IShape& m_shape;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_INTERSECTION
