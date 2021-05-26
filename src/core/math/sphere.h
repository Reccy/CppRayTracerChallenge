#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE

#include "shape.h"
#include "point.h"
#include "transform.h"

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
		/// Calculates and returns the normal at the passed in position for this sphere
		/// </summary>
		/// <param name="position">The position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normal(const Point position) const;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE
