#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE

#include "point.h"
#include "transform.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A 3D Sphere
	/// </summary>
	class Sphere
	{
	public:
		/// <summary>
		/// Creates a unit sphere at position 0, 0, 0
		/// </summary>
		Sphere();

		/// <summary>
		/// Returns the position of the sphere
		/// </summary>
		/// <returns>Position of the sphere</returns>
		Point position() const;

		/// <summary>
		/// Transforms the Sphere
		/// </summary>
		/// <param name="transform">The transform to apply to the sphere</param>
		void transform(Transform transform);

		/// <summary>
		/// Returns a copy of the Sphere's Transform
		/// </summary>
		/// <returns>The Sphere's Transform</returns>
		const Transform transform() const;

		/// <summary>
		/// Calculates and returns the normal at the passed in position for this sphere
		/// </summary>
		/// <param name="position">The position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normal(const Point position) const;
	private:
		Point m_position;
		Transform m_transform;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE
