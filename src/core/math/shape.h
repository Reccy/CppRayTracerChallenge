#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_SHAPE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_SHAPE

#include "i_shape.h"

namespace CppRayTracerChallenge::Core::Math
{
	class Shape : public IShape
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
	protected:
		Transform m_transform;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_SHAPE
