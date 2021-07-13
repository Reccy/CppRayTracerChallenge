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
		/// Creates a Cylinder at position 0, 0, 0 with defined minimum and maximum bounds
		/// </summary>
		/// <param name="minimum">Minimum bounds</param>
		/// <param name="maximum">Maximum bounds</param>
		Cylinder(double minimum, double maximum);

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

		/// <summary>
		/// Returns the maximum bounds of the Cylinder
		/// </summary>
		/// <returns>Max length</returns>
		double maximum() const;

		/// <summary>
		/// Sets the maximum bounds of the cylinder
		/// </summary>
		/// <param name="maximum">The maximum bounds to set</param>
		void maximum(double maximum);

		/// <summary>
		/// Returns the minimum bounds of the cylinder
		/// </summary>
		/// <returns>Min bounds</returns>
		double minimum() const;

		/// <summary>
		/// Sets the minimum bounds of the cylinder
		/// </summary>
		/// <param name="minimum">The minimum bounds to set</param>
		void minimum(double minimum);
	private:
		double m_minimum;
		double m_maximum;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_CYLINDER
