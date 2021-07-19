#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX

#include "../math/point.h"

namespace CppRayTracerChallenge::Core::Renderer
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
		BoundingBox(Math::Point min, Math::Point max);

		/// <summary>
		/// Returns the minimum bounds
		/// </summary>
		/// <returns>Minimum bounds</returns>
		Math::Point min() const;

		/// <summary>
		/// Returns the maximum bounds
		/// </summary>
		/// <returns>Maximum bounds</returns>
		Math::Point max() const;

		/// <summary>
		/// Resizes box to include passed in point
		/// </summary>
		/// <param name="position">The position to include</param>
		void resizeToFit(Math::Point position);
	private:
		Math::Point m_min;
		Math::Point m_max;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_BOUNDING_BOX