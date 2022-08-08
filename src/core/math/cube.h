#pragma once
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
		/// Calculates if the local space ray intersects with the cube.
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the cube</param>
		/// <returns>The Intersections</returns>
		const Intersections intersectLocal(Ray ray) const override;

		/// <summary>
		/// Returns the bounding box of the cube
		/// </summary>
		/// <returns>Bounding Box that contains the cube</returns>
		const BoundingBox bounds() const override;
	private:
		struct CheckAxisResult
		{
			double tMin;
			double tMax;
		};

		CheckAxisResult checkAxis(double origin, double direction) const;
	};
}
