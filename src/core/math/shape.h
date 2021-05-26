#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_SHAPE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_SHAPE

#include "point.h"
#include "transform.h"

namespace CppRayTracerChallenge::Core::Math
{
	class Shape
	{
	public:
		/// <summary>
		/// Transforms the shape
		/// </summary>
		/// <param name="transform">The transform to apply to the shape</param>
		void transform(Transform transform);

		/// <summary>
		/// Returns a copy of the Shape's transform
		/// </summary>
		/// <returns>The Shape's transform</returns>
		const Transform transform() const;

		/// <summary>
		/// Calculates and returns the normal at the passed in position for this shape
		/// </summary>
		/// <param name="position">The position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		virtual const Vector normal(const Point position) const = 0;
	protected:
		Point m_position;
		Transform m_transform;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_SHAPE
