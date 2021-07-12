#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_CYLINDER
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_CYLINDER

#include "shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	class Cylinder : public Shape
	{
	public:
		/// <summary>
		/// Creates a Cylinder at position 0, 0, 0
		/// </summary>
		Cylinder();

		/// <summary>
		/// Calculates and returns the normal at the passed in local space position for this cylinder
		/// </summary>
		/// <param name="position">The local space position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normalLocal(const Point position) const override;

		/// <summary>
		/// Calculates if the local space ray intersects with the cylinder.
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the sphere</param>
		/// <returns>The Intersections</returns>
		const Intersections intersectLocal(Ray ray) const override;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_CYLINDER
