#pragma once
#include "shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A 3D Torus
	/// </summary>
	class Torus : public Shape
	{
	public:
		/// <summary>
		/// Creates a torus at position 0, 0, 0 with tube radius 1 and sweep radius 1
		/// </summary>
		Torus();

		/// <summary>
		/// Creates a torus at position 0, 0, 0
		/// </summary>
		/// <param name="tubeRadius">The radius of the tube</param>
		/// <param name="sweepRadius">The radius from the center of the torus to the center of the tube</param>
		Torus(double tubeRadius, double sweepRadius);

		/// <summary>
		/// Calculates and returns the normal at the passed in local space position for this torus
		/// </summary>
		/// <param name="position">The local space position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normalLocal(const Point position) const override;

		/// <summary>
		/// Calculates if the local space ray intersects with the torus.
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the torus</param>
		/// <returns>The Intersections</returns>
		const Intersections intersectLocal(Ray ray) const override;

		/// <summary>
		/// Returns the bounding box of the torus
		/// </summary>
		/// <returns>Bounding Box that contains the torus</returns>
		const BoundingBox bounds() const override;
	private:
		double m_sweepRadius;
		double m_tubeRadius;
	};
}
