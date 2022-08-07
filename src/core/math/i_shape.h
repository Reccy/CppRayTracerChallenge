#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_I_SHAPE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_I_SHAPE

#include "RML.h"
#include "intersections.h"
#include "bounding_box.h"
#include "ray.h"

namespace CppRayTracerChallenge::Core::Math
{
	class Intersections;
	class IShape
	{
	public:
		virtual ~IShape() {};

		/// <summary>
		/// Transforms the shape
		/// </summary>
		/// <param name="transform">The transform to apply to the shape</param>
		virtual void transform(RML::Transform transform) = 0;

		/// <summary>
		/// Returns a reference to the Shape's transform
		/// </summary>
		/// <returns>The Shape's transform</returns>
		virtual RML::Transform& transform() = 0;

		/// <summary>
		/// Calculates and returns the normal at the passed in world space position for this shape
		/// </summary>
		/// <param name="position">The world space position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		virtual const Vector normal(const Point position) const = 0;

		/// <summary>
		/// Calculates and returns the normal at the passed in local space position for this shape
		/// </summary>
		/// <param name="position">The local space position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		virtual const Vector normalLocal(const Point position) const = 0;

		/// <summary>
		/// Calculates if the world space ray intersects with the shape.
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the shape</param>
		/// <returns>The Intersections</returns>
		virtual const Intersections intersect(Ray ray) const = 0;

		/// <summary>
		/// Calculates if the local space ray intersects with the shape.
		/// </summary>
		/// <param name="ray">The local space ray to intersect with the shape</param>
		/// <returns>The Intersections</returns>
		virtual const Intersections intersectLocal(Ray ray) const = 0;

		/// <summary>
		/// Returns the bounding box of the shape
		/// </summary>
		/// <returns>Bounding Box that contains the shape</returns>
		virtual const BoundingBox bounds() const = 0;

		/// <summary>
		/// Returns the bounding box of the shape in the space of the parent shape
		/// </summary>
		/// <returns>Bounding Box that contains the shape, in the space of the parent</returns>
		virtual const BoundingBox parentSpaceBounds() const = 0;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_I_SHAPE
