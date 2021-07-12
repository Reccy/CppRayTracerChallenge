#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_CUBE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_CUBE

#include "shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	class Cube : public Shape
	{
	public:
		/// <summary>
		/// Creates an Axis Aligned Bounding Box (AABB) at position 0, 0, 0
		/// </summary>
		Cube();

		/// <summary>
		/// Calculates and returns the normal at the passed in local space position for this cube
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

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_CUBE