#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_TRANSFORM
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_TRANSFORM

#include <stack>
#include "matrix.h"

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
		Transform();

		/// <summary>
		/// Translates the Transform along x, y, z co-ordinates
		/// </summary>
		/// <param name="x">X translation</param>
		/// <param name="y">Y translation</param>
		/// <param name="z">Z translation</param>
		/// <returns>The Transform after being translated</returns>
		Transform translate(const double x, const double y, const double z);

		/// <summary>
		/// Scales the Transform along x, y, z axes
		/// </summary>
		/// <param name="x">Scale along the X axis</param>
		/// <param name="y">Scale along the Y axis</param>
		/// <param name="z">Scale along the Z axis</param>
		/// <returns>The Transform after being scaled</returns>
		Transform scale(const double x, const double y, const double z);

		/// <summary>
		/// Rotates the Transform along the x, y and z axes
		/// </summary>
		/// <param name="x">Rotate around the X axis in degrees</param>
		/// <param name="y">Rotate around the Y axis in degrees</param>
		/// <param name="z">Rotate around the Z axis in degrees</param>
		/// <returns>The Transform after being rotated</returns>
		Transform rotate(const double x, const double y, const double z);

		/// <summary>
		/// Shears the Transform by moving a component in relation to another
		/// </summary>
		/// <param name="xY">Move X in proportion to Y</param>
		/// <param name="xZ">Move X in proportion to Z</param>
		/// <param name="yX">Move Y in proportion to X</param>
		/// <param name="yz">Move Y in proportion to Z</param>
		/// <param name="zX">Move Z in proportion to X</param>
		/// <param name="zY">Move Z in proportion to Y</param>
		/// <returns>The Transform after being sheared</returns>
		Transform shear(const double xY, const double xZ, const double yX, const double yZ, const double zX, const double zY);

		/// <summary>
		/// Inverts the Transform
		/// </summary>
		/// <returns>The Transform after being inverted</returns>
		Transform invert();

		/// <summary>
		/// Multiplies the Tuple by the Transform, and returns the transformed Tuple
		/// </summary>
		/// <param name="tuple">The Tuple to transform</param>
		/// <returns>The Tuple after being transformed</returns>
		Tuple<double> operator*(const Tuple<double>& tuple);
	private:
		std::stack<Matrix<double>> m_matrices;
		Matrix<double> m_matrix;

		void calculateMatrix();
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_TRANSFORM
