#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_CONE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_CONE

#include "shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	class Cone : public Shape
	{
	public:
		/// <summary>
		/// Creates a Cone at position 0, 0, 0
		/// </summary>
		Cone();

		/// <summary>
		/// Creates a Cone at position 0, 0, 0 with defined minimum and maximum bounds
		/// </summary>
		/// <param name="minimum">Minimum</param>
		/// <param name="maximum">Maximum</param>
		Cone(double minimum, double maximum);

		/// <summary>
		/// Creates a Cone at position 0, 0, 0 with defined minimum and maximum bounds, with defined caps
		/// </summary>
		/// <param name="minimum">Minimum bounds</param>
		/// <param name="maximum">Maximum bounds</param>
		/// <param name="closed">If the cone is capped</param>
		Cone(double minimum, double maximum, bool closed);

		/// <summary>
		/// Calculates and returns the normal at the passed in local space position for this cone
		/// </summary>
		/// <param name="position">The local space position of the normal vactor</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normalLocal(const Point position) const override;

		/// <summary>
		/// Calculates if the local space ray intersects with the cone
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the cone</param>
		/// <returns>The Intersections</returns>
		const Intersections intersectLocal(Ray ray) const override;

		/// <summary>
		/// Returns the maximum bounds of the Cone
		/// </summary>
		/// <returns>Max length</returns>
		double maximum() const;

		/// <summary>
		/// Sets the maximum bounds of the Cone
		/// </summary>
		/// <param name="maximum">The maximum bounds to set</param>
		void maximum(double maximum);

		/// <summary>
		/// Returns the minimum bounds of the Cone
		/// </summary>
		/// <returns>Min bounds</returns>
		double minimum() const;

		/// <summary>
		/// Sets the minimum bounds of the Cone
		/// </summary>
		/// <param name="minimum">The minimum bounds to set</param>
		void minimum(double minimum);

		/// <summary>
		/// Returns true if the Cone is closed
		/// </summary>
		/// <returns>If the cylinder is closed</returns>
		bool closed() const;

		/// <summary>
		/// Sets if the Cone is closed
		/// </summary>
		/// <param name="closed">The closed value</param>
		void closed(bool closed);
	private:
		double m_minimum;
		double m_maximum;
		bool m_closed;
	};
};

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_CONE
