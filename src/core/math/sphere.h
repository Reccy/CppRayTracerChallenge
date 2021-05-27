#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE

#include "shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A 3D Sphere
	/// </summary>
	class Sphere : public Shape
	{
	public:
		/// <summary>
		/// Creates a unit sphere at position 0, 0, 0
		/// </summary>
		Sphere();

		/// <summary>
		/// Calculates and returns the normal at the passed in local space position for this sphere
		/// </summary>
		/// <param name="position">The local space position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normalLocal(const Point position) const override;

		/// <summary>
		/// Calculates if the local space ray intersects with the sphere.
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the sphere</param>
		/// <returns>The Intersections</returns>
		const Intersections intersectLocal(Ray ray) const override;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE
