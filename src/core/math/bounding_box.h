#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX

#include "point.h"
#include "transform.h"
#include "ray.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A bounding box used to optimise render times using the Bounding Volume Hierarchy method.
	/// http://raytracerchallenge.com/bonus/bounding-boxes.html
	/// </summary>
	class BoundingBox
	{
	public:
		/// <summary>
		/// Creates a Bounding Box with no defined volume.
		/// </summary>
		BoundingBox();

		/// <summary>
		/// Creates a Bounding Box with defined volume.
		/// </summary>
		/// <param name="min">Minimum bounds</param>
		/// <param name="max">Maximum bounds</param>
		BoundingBox(Point min, Point max);

		/// <summary>
		/// Calculates if the ray intersects with the Bounding Box.
		/// </summary>
		/// <param name="ray">The ray to intersect with the Bounding Box</param>
		/// <returns>If the ray intersected or not</returns>
		bool intersects(Ray ray) const;

		/// <summary>
		/// Returns the minimum bounds
		/// </summary>
		/// <returns>Minimum bounds</returns>
		Point min() const;

		/// <summary>
		/// Returns the maximum bounds
		/// </summary>
		/// <returns>Maximum bounds</returns>
		Point max() const;

		/// <summary>
		/// Adds point to box and resizes it to include passed in point
		/// </summary>
		/// <param name="position">The position to include</param>
		void add(Point position);

		/// <summary>
		/// Combines other bounding box into this one
		/// </summary>
		/// <param name="other"></param>
		void add(const BoundingBox& other);

		/// <summary>
		/// Checks if the position is within the volume of the bounding box
		/// </summary>
		/// <param name="position">The position in local space to check</param>
		/// <returns>If the position is within the bounds of the box</returns>
		bool contains(const Point& position) const;

		/// <summary>
		/// Checks if the passed in box fits within the volume of this bounding box.
		/// </summary>
		/// <param name="other">The other bounding box to check</param>
		/// <returns>If the box is within the bounds of this box</returns>
		bool contains(const BoundingBox& other) const;

		/// <summary>
		/// Transforms the bounding box
		/// </summary>
		/// <param name="transform">The transform to apply to the bounding box</param>
		void transform(Transform transform);

		/// <summary>
		/// Returns a copy of the Box's transform
		/// </summary>
		/// <returns>The Box's transform</returns>
		const Transform transform() const;
	private:
		struct CheckAxisResult
		{
			double tMin;
			double tMax;
		};

		CheckAxisResult checkAxis(double origin, double direction, double min, double max) const;

		Transform m_transform;
		const Point m_initialMin;
		const Point m_initialMax;
		Point m_min;
		Point m_max;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX
