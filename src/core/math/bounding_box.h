#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX

#include "point.h"

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
		bool containsPoint(const Point& position) const;
	private:
		Point m_min;
		Point m_max;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX
