#pragma once

#include "RML.h"
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
		/// Returns a reference to the Shape's transform
		/// </summary>
		/// <returns>The Shape's transform</returns>
		Transform& transform();

		/// <summary>
		/// Calculates and returns the normal at the passed in position for this shape
		/// </summary>
		/// <param name="position">The position of the normal vector</param>
		/// <returns>The normal vector at the position</returns>
		const RML::Vector normal(const RML::Point position) const override;

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
