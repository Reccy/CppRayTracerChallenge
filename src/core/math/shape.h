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

		/// <summary>
		/// Calculates and returns the normal at the passed in position for this shape
		/// </summary>
		/// <param name="position">The position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		const Vector normal(const Point position) const override;

		/// <summary>
		/// Calculates if the ray intersects with the sphere.
		/// </summary>
		/// <param name="ray">The ray to intersect with the sphere</param>
		/// <returns>The Intersections</returns>
		const Intersections intersect(Ray ray) const override;

		/// <summary>
		/// Returns the bounding box of the shape in the space of the parent shape
		/// </summary>
		/// <returns>Bounding Box that contains the shape, in the space of the parent</returns>
		const BoundingBox parentSpaceBounds() const override;
	protected:
		Transform m_transform;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_SHAPE
