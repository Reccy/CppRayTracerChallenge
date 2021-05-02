#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE

#include "point.h"
#include "i_intersectable.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A 3D Sphere
	/// </summary>
	class Sphere : public IIntersectable
	{
	public:
		/// <summary>
		/// Creates a unit sphere at position 0, 0, 0
		/// </summary>
		Sphere();

		/// <summary>
		/// Returns the position of the sphere
		/// </summary>
		/// <returns>Position of the sphere</returns>
		virtual Point position() const;

		/// <summary>
		/// Returns the radius of the sphere
		/// </summary>
		/// <returns>Radius of the sphere</returns>
		double radius() const;
	private:
		Point m_position;
		double m_radius;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_SPHERE
