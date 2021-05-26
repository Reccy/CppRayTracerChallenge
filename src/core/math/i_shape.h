#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_I_SHAPE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_I_SHAPE

#include "transform.h"
#include "vector.h"
#include "point.h"

namespace CppRayTracerChallenge::Core::Math
{
	class IShape
	{
	public:
		/// <summary>
		/// Transforms the shape
		/// </summary>
		/// <param name="transform">The transform to apply to the shape</param>
		virtual void transform(Transform transform) = 0;

		/// <summary>
		/// Returns a copy of the Shape's transform
		/// </summary>
		/// <returns>The Shape's transform</returns>
		virtual const Transform transform() const = 0;

		/// <summary>
		/// Calculates and returns the normal at the passed in position for this shape
		/// </summary>
		/// <param name="position">The position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		virtual const Vector normal(const Point position) const = 0;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_I_SHAPE
