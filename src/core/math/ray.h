#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_RAY
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_RAY

#include <vector>
#include "point.h"
#include "vector.h"
#include "sphere.h"
#include "intersections.h"
#include "transform.h"

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

		/// <summary>
		/// Calculates and returns the position at 't' distance from the origin
		/// </summary>
		/// <param name="t">The distance from the origin in which to calculate the new position</param>
		/// <returns>The position 't' distance from the origin</returns>
		Point position(const double t) const;

		/// <summary>
		/// Creates a copy of this ray and transforms it
		/// </summary>
		/// <param name="transform">The transform to apply to the new ray</param>
		/// <returns>A transformed ray</returns>
		Ray transform(const Transform& transform) const;

		/// <summary>
		/// Calculates the t values where this ray intersects a sphere
		/// </summary>
		/// <param name="sphere">Reference to the Sphere to calculate the intersections on</param>
		/// <returns>A list of intersections where the ray intersects the sphere</returns>
		Intersections intersect_sphere(const Sphere& sphere) const;
	private:
		Point m_origin;
		Vector m_direction;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_RAY
