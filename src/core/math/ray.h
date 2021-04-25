#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_RAY
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_RAY

#include "point.h"
#include "vector.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// Represents a Ray composed of an origin point and a direction vector.
	/// The direction vector can also be used to represent the "speed" of the ray.
	/// </summary>
	class Ray
	{
	public:
		Ray() = delete;

		/// <summary>
		/// Creates a new way at origin pointing in direction
		/// </summary>
		/// <param name="origin">The origin point of the ray</param>
		/// <param name="direction">The vector direction of the ray</param>
		Ray(Point origin, Vector direction);

		/// <summary>
		/// Returns a copy of the Ray origin
		/// </summary>
		/// <returns>Ray origin</returns>
		Point origin() const;

		/// <summary>
		/// Returns a copy of the Ray vector
		/// </summary>
		/// <returns>Ray vector</returns>
		Vector direction() const;
	private:
		Point m_origin;
		Vector m_direction;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_RAY
