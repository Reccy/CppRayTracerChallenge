#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_TRANSFORM
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_TRANSFORM

#include "math/matrix.h"
#include "math/point.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// Represents a Transformation Matrix that can be used to transform vectors and points
	/// </summary>
	class Transform
	{
	public:
		/// <summary>
		/// Constructs a Transform with the identity matrix
		/// </summary>
		Transform() : m_matrix(Matrix<double>::identity(4)) { };

		/// <summary>
		/// Translates the Transform along x, y, z co-ordinates
		/// </summary>
		/// <param name="x">X translation</param>
		/// <param name="y">Y translation</param>
		/// <param name="z">Z translation</param>
		/// <returns>The Transform after being translated</returns>
		Transform translate(const double x, const double y, const double z)
		{
			m_matrix(0, 3) = m_matrix(0, 3) + x;
			m_matrix(1, 3) = m_matrix(1, 3) + y;
			m_matrix(2, 3) = m_matrix(2, 3) + z;

			return *this;
		};

		/// <summary>
		/// Scales the Transform along x, y, z axes
		/// </summary>
		/// <param name="x">Scale along the X axis</param>
		/// <param name="y">Scale along the Y axis</param>
		/// <param name="z">Scale along the Z axis</param>
		/// <returns>The Transform after being scaled</returns>
		Transform scale(const double x, const double y, const double z)
		{
			m_matrix(0, 0) = m_matrix(0, 0) * x;
			m_matrix(1, 1) = m_matrix(1, 1) * y;
			m_matrix(2, 2) = m_matrix(2, 2) * z;

			return *this;
		}

		/// <summary>
		/// Inverts the Transform
		/// </summary>
		/// <returns>The Transform after being inverted</returns>
		Transform invert()
		{
			m_matrix = m_matrix.invert();

			return *this;
		};

		/// <summary>
		/// Multiplies the Tuple by the Transform, and returns the transformed Tuple
		/// </summary>
		/// <param name="tuple">The Tuple to transform</param>
		/// <returns>The Tuple after being transformed</returns>
		Tuple<double> operator*(const Tuple<double>& tuple) const
		{
			return m_matrix * tuple;
		};
	private:
		Matrix<double> m_matrix;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_TRANSFORM
