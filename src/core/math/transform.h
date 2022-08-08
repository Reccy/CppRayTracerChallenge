#pragma once

#include <stack>
#include "RML.h"

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
		const Transform translate(const double x, const double y, const double z) const;

		/// <summary>
		/// Scales the Transform along x, y, z axes
		/// </summary>
		/// <param name="x">Scale along the X axis</param>
		/// <param name="y">Scale along the Y axis</param>
		/// <param name="z">Scale along the Z axis</param>
		/// <returns>The Transform after being scaled</returns>
		const Transform scale(const double x, const double y, const double z) const;

		/// <summary>
		/// Rotates the Transform along the x, y and z axes
		/// </summary>
		/// <param name="x">Rotate around the X axis in degrees</param>
		/// <param name="y">Rotate around the Y axis in degrees</param>
		/// <param name="z">Rotate around the Z axis in degrees</param>
		/// <returns>The Transform after being rotated</returns>
		const Transform rotate(const double x, const double y, const double z) const;

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
		const Transform shear(const double xY, const double xZ, const double yX, const double yZ, const double zX, const double zY) const;

		/// <summary>
		/// Transposes the matrix
		/// </summary>
		/// <returns>The Transform after being transposed</returns>
		const Transform transpose() const;

		/// <summary>
		/// Inverts the Transform
		/// </summary>
		/// <returns>The Transform after being inverted</returns>
		const Transform invert() const;

		/// <summary>
		/// Returns the transform as an actual matrix
		/// </summary>
		/// <returns>The transform matrix</returns>
		const RML::Matrix<double, 4, 4> matrix() const;

		/// <summary>
		/// Multiplies the Tuple by the Transform, and returns the transformed Tuple
		/// </summary>
		/// <param name="tuple">The Tuple to transform</param>
		/// <returns>The Tuple after being transformed</returns>
		RML::Tuple4<double> operator*(const RML::Tuple4<double>& tuple) const;

		bool operator==(const Transform& other) const;
		bool operator!=(const Transform& other) const;
	private:
		std::stack<RML::Matrix<double, 4, 4>> m_matrices;
		RML::Matrix<double, 4, 4> m_matrix;

		void calculateMatrix();
	};
}
