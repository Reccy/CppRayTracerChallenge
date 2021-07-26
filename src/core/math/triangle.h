#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_TRIANGLE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_TRIANGLE

#include "shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A triangle primitive
	/// </summary>
	class Triangle : public Shape
	{
	public:
		Triangle() = delete;

		Triangle(Point p1, Point p2, Point p3);

		/// <summary>
		/// Returns the normal vector for this triangle
		/// </summary>
		/// <param name="position">The local space position of the normal vector, does not matter since this is a triangle</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normalLocal(const Point position) const override;

		/// <summary>
		/// Returns the normal vector for this triangle
		/// </summary>
		/// <returns>The normal vector</returns>
		const Vector normal() const;

		/// <summary>
		/// Calculates if the local space ray intersects with the triangle
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the triangle</param>
		/// <returns>The Intersections</returns>
		const Intersections intersectLocal(Ray ray) const override;

		/// <summary>
		/// Returns the bounding box of the triangle
		/// </summary>
		/// <returns>Bounding Box that contains the triangle</returns>
		const BoundingBox bounds() const override;

		Point p1() const;

		Point p2() const;

		Point p3() const;

		Vector e1() const;

		Vector e2() const;
	protected:
		Point m_p1, m_p2, m_p3;
		Vector m_e1, m_e2;
		Vector m_normal;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_TRIANGLE
