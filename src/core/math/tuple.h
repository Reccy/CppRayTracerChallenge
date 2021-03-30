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
		Tuple(const float x, const float y, const float z, const float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

		/// <summary>
		/// Creates a new point
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		/// <returns>Tuple that is a point</returns>
		static Tuple buildPoint(const float x, const float y, const float z);

		/// <summary>
		/// Creates a new vector
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		/// <returns>Tuple that is a vector</returns>
		static Tuple buildVector(const float x, const float y, const float z);

		/// <summary>
		/// Creates a zero vector
		/// </summary>
		/// <returns>Tuple that is a vector</returns>
		static Tuple zeroVector();

		/// <summary>
		/// Returns a readonly X component
		/// </summary>
		float getX() const;

		/// <summary>
		/// Returns a readonly Y component
		/// </summary>
		float getY() const;

		/// <summary>
		/// Returns a readonly Z component
		/// </summary>
		float getZ() const;

		/// <summary>
		/// Returns a readonly W component
		/// </summary>
		float getW() const;

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

		bool operator==(const Tuple& other) const;
		bool operator!=(const Tuple& other) const;
		Tuple operator+(const Tuple& other);
		Tuple operator-(const Tuple& other);
		Tuple operator-() const;
		Tuple operator*(const float scalar);
		Tuple operator/(const float scalar);
		friend std::ostream& operator<<(std::ostream& os, const Tuple& tuple);
	private:
		float m_x, m_y, m_z, m_w;
	};
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
