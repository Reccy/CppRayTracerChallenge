#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE

#include <iostream>

namespace CppRayTracerChallenge::Core::Math {
	/// <summary>
	/// A Tuple with x, y, z and w components
	/// </summary>
	class Tuple {
	public:
		Tuple() = delete;

		/// <summary>
		/// Creates a tuple with x, y, z and w components
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		/// <param name="w">W component. 1 is a point, 2 is a tuple</param>
		Tuple(const double x, const double y, const double z, const double w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

		/// <summary>
		/// Creates a new point
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		/// <returns>Tuple that is a point</returns>
		static Tuple buildPoint(const double x, const double y, const double z);

		/// <summary>
		/// Creates a new vector
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		/// <returns>Tuple that is a vector</returns>
		static Tuple buildVector(const double x, const double y, const double z);

		/// <summary>
		/// Creates a zero vector
		/// </summary>
		/// <returns>Tuple that is a vector</returns>
		static Tuple zeroVector();

		/// <summary>
		/// Returns a readonly X component
		/// </summary>
		double getX() const;

		/// <summary>
		/// Returns a readonly Y component
		/// </summary>
		double getY() const;

		/// <summary>
		/// Returns a readonly Z component
		/// </summary>
		double getZ() const;

		/// <summary>
		/// Returns a readonly W component
		/// </summary>
		double getW() const;

		/// <summary>
		/// Returns if a tuple is a point
		/// </summary>
		/// <returns>True if point, False otherwise</returns>
		bool isPoint() const;

		/// <summary>
		/// Returns if a tuple is a vector
		/// </summary>
		/// <returns>True if vector, False otherwise</returns>
		bool isVector() const;

		/// <summary>
		/// Calculates and returns the magnitude of the tuple
		/// </summary>
		/// <returns>Magnitude of the tuple</returns>
		double magnitude() const;

		/// <summary>
		/// Calculates and returns a normalized version of the tuple
		/// </summary>
		/// <returns>Normalized tuple</returns>
		Tuple normalize() const;

		bool operator==(const Tuple& other) const;
		bool operator!=(const Tuple& other) const;
		Tuple operator+(const Tuple& other);
		Tuple operator-(const Tuple& other);
		Tuple operator-() const;
		Tuple operator*(const double scalar);
		Tuple operator/(const double scalar);
		friend std::ostream& operator<<(std::ostream& os, const Tuple& tuple);
	private:
		double m_x, m_y, m_z, m_w;
	};
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
