#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_VECTOR
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_VECTOR

#include "tuple.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A Vector in Euclidean Space
	/// </summary>
	class Vector : public Tuple<double> {
	public:
		/// <summary>
		/// Constructs a Vector with components 0, 0, 0
		/// Note that this has an undefined direction!
		/// </summary>
		Vector();

		/// <summary>
		/// Constructs a Vector from a tuple, by copying x, y and z components.
		/// W component is set to 0.
		/// </summary>
		/// <param name="tuple">The tuple to convert to a Vector</param>
		Vector(const Tuple tuple);

		/// <summary>
		/// Constructs a Vector with x, y and z components
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		Vector(const double x, const double y, const double z);

		/// <summary>
		/// Creates a vector where all components are 0
		/// </summary>
		/// <returns>Zero Vector</returns>
		static Vector zero();

		/// <summary>
		/// Performs a cross product operation and returns the result
		/// </summary>
		/// <param name="a">The first vector</param>
		/// <param name="b">The second vector</param>
		/// <returns>A vector cross product</returns>
		static Vector cross(const Vector& a, const Vector& b);

		/// <summary>
		/// Performs a dot product operation and returns the result
		/// </summary>
		/// <param name="a">The first vector</param>
		/// <param name="b">The second vector</param>
		/// <returns>A scalar dot product</returns>
		static double dot(const Vector& a, const Vector& b);

		/// <summary>
		/// Reflects the vector off the normal
		/// </summary>
		/// <param name="vector">The vector to reflect</param>
		/// <param name="normal">The normal vector</param>
		/// <returns>A vector reflecting off the normal</returns>
		static Vector reflect(const Vector& vector, const Vector& normal);

		/// <summary>
		/// Calculates and returns the magnitude of the vector
		/// </summary>
		/// <returns>Magnitude of the vector</returns>
		double magnitude() const;

		/// <summary>
		/// Calculates and returns a normalized version of the vector
		/// </summary>
		/// <returns>Normalized vector</returns>
		Vector normalize() const;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_VECTOR
