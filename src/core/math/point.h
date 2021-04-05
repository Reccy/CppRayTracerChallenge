#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_POINT
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_POINT

#include "vector.h"
#include "tuple.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// Point in Euclidean Space
	/// </summary>
	class Point
	{
	public:
		Point() = delete;

		/// <summary>
		/// Constructs a Point from a tuple, by copying x, y and z components.
		/// W component is set to 1.
		/// </summary>
		/// <param name="tuple">The tuple to convert to a Point</param>
		Point(const Tuple tuple) : m_tuple(Tuple(tuple.x(), tuple.y(), tuple.z(), 1)) {};

		/// <summary>
		/// Constructs a Point with x, y and z components
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		Point(const double x, const double y, const double z) : m_tuple(Tuple(x, y, z, 1)) {};

		/// <summary>
		/// Returns a const x component
		/// </summary>
		double x() const;

		/// <summary>
		/// Returns a const y component
		/// </summary>
		double y() const;

		/// <summary>
		/// Returns a const z component
		/// </summary>
		double z() const;

		/// <summary>
		/// Returns a const w component
		/// </summary>
		double w() const;

		bool operator==(const Point& other) const;
		bool operator!=(const Point& other) const;
		Point operator+(const Point& other) const;
		Point operator+(const Vector& other) const;
		Point operator-(const Vector& other) const;
		Vector operator-(const Point& other) const;
		Point operator-() const;
		Point operator*(const double scalar) const;
		Point operator/(const double scalar) const;
		friend std::ostream& operator<<(std::ostream& os, const Point& vector);
	private:
		Tuple m_tuple;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_POINT
