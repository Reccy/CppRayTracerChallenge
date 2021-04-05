#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE

#include <iostream>
#include "i_cartesian.h"

namespace CppRayTracerChallenge::Core::Math {
	/// <summary>
	/// A Tuple with x, y, z and w components
	/// </summary>
	class Tuple : public ICartesian<double> {
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
		/// Returns a const X component
		/// </summary>
		double x() const override;

		/// <summary>
		/// Returns a const Y component
		/// </summary>
		double y() const override;

		/// <summary>
		/// Returns a const Z component
		/// </summary>
		double z() const override;
		
		/// <summary>
		/// Returns a const W component
		/// </summary>
		double w() const override;

		bool operator==(const Tuple& other) const;
		bool operator!=(const Tuple& other) const;
		Tuple operator+(const Tuple& other) const;
		Tuple operator-(const Tuple& other) const;
		Tuple operator-() const;
		Tuple operator*(const double scalar) const;
		Tuple operator/(const double scalar) const;
		friend std::ostream& operator<<(std::ostream& os, const Tuple& tuple);
	private:
		double m_x, m_y, m_z, m_w;
	};
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
