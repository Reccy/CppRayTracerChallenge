#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_PLANE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_PLANE

#include "shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A 3D Plane across the xz axes
	/// </summary>
	class Plane : public Shape
	{
	public:
		/// <summary>
		/// Creates a plane
		/// </summary>
		Plane();

		/// <summary>
		/// Calculates and returns the normal at the passed in local space position for this plane
		/// </summary>
		/// <param name="position">The local space position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normalLocal(const Point position) const override;

		/// <summary>
		/// Calculates if the local space ray intersects with the plane.
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the plane</param>
		/// <returns>The Intersections</returns>
		const Intersections intersectLocal(const Ray ray) const override;

		/// <summary>
		/// Returns the bounding box of the plane
		/// </summary>
		/// <returns>Bounding Box that contains the plane</returns>
		const BoundingBox bounds() const override;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_PLANE
