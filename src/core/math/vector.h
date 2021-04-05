#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_VECTOR
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_VECTOR

#include "tuple.h"
#include "i_cartesian.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// A Vector in Euclidean Space
	/// </summary>
	class Vector : public ICartesian<double> {
	public:
		Vector() = delete;

		/// <summary>
		/// Constructs a Vector from a tuple, by copying x, y and z components.
		/// W component is set to 0.
		/// </summary>
		/// <param name="tuple">The tuple to convert to a Vector</param>
		Vector(const Tuple tuple) : m_tuple(Tuple(tuple.x(), tuple.y(), tuple.z(), 0)) {};

		/// <summary>
		/// Constructs a Vector with x, y and z components
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		Vector(const double x, const double y, const double z) : m_tuple(Tuple(x, y, z, 0)) {};

		/// <summary>
		/// Creates a vector where all components are 0
		/// </summary>
		/// <returns>Zero Vector</returns>
		static Vector zero();

		/// <summary>
		/// Returns a const x component
		/// </summary>
		double x() const override;

		/// <summary>
		/// Returns a const y component
		/// </summary>
		double y() const override;

		/// <summary>
		/// Returns a const z component
		/// </summary>
		double z() const override;

		/// <summary>
		/// Returns a const w component
		/// </summary>
		double w() const override;

		/// <summary>
		/// Performs a cross product operation and returns the result
		/// </summary>
		/// <param name="a">The first vector</param>
		/// <param name="b">The second vector</param>
		/// <returns>A vector cross product</returns>
		static Vector cross(const Vector a, const Vector b);

		/// <summary>
		/// Performs a dot product operation and returns the result
		/// </summary>
		/// <param name="a">The first vector</param>
		/// <param name="b">The second vector</param>
		/// <returns>A scalar dot product</returns>
		static double dot(const Vector a, const Vector b);

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

		bool operator==(const Vector& other) const;
		bool operator!=(const Vector& other) const;
		Vector operator+(const Vector& other) const;
		Vector operator-(const Vector& other) const;
		Vector operator-() const;
		Vector operator*(const double scalar) const;
		Vector operator/(const double scalar) const;
		friend std::ostream& operator<<(std::ostream& os, const Vector& vector);
	public:
		Tuple m_tuple;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_VECTOR
