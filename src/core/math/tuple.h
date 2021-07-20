#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE

#include <iostream>
#include <type_traits>
#include "comparison.h"

namespace CppRayTracerChallenge::Core::Math {
	/// <summary>
	/// A Tuple with x, y, z and w components
	/// </summary>
	template<typename T>
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
		Tuple(const T x, const T y, const T z, const T w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

		/// <summary>
		/// Returns a const X component
		/// </summary>
		T x() const
		{
			return this->m_x;
		};

		/// <summary>
		/// Returns a const Y component
		/// </summary>
		T y() const
		{
			return this->m_y;
		};

		/// <summary>
		/// Returns a const Z component
		/// </summary>
		T z() const
		{
			return this->m_z;
		};
		
		/// <summary>
		/// Returns a const W component
		/// </summary>
		T w() const
		{
			return this->m_w;
		};

		bool operator==(const Tuple& other) const
		{
			return	Comparison::equal(this->m_x, other.m_x) &&
				Comparison::equal(this->m_y, other.m_y) &&
				Comparison::equal(this->m_z, other.m_z) &&
				Comparison::equal(this->m_w, other.m_w);
		};

		bool operator!=(const Tuple& other) const
		{
			return !(*this == other);
		};

		Tuple operator+(const Tuple& other) const
		{
			return Tuple(this->m_x + other.m_x, this->m_y + other.m_y, this->m_z + other.m_z, this->m_w + other.m_w);
		};

		Tuple& operator+=(const Tuple& other)
		{
			Tuple res = *this + other;
			this->m_x = res.m_x;
			this->m_y = res.m_y;
			this->m_z = res.m_z;
			this->m_w = res.m_w;
			return *this;
		}

		Tuple operator-(const Tuple& other) const
		{
			return Tuple(this->m_x - other.m_x, this->m_y - other.m_y, this->m_z - other.m_z, this->m_w - other.m_w);
		};

		Tuple& operator-=(const Tuple& other)
		{
			Tuple res = *this - other;
			this->m_x = res.m_x;
			this->m_y = res.m_y;
			this->m_z = res.m_z;
			this->m_w = res.m_w;
			return *this;
		}

		Tuple operator-() const
		{
			return Tuple(0, 0, 0, 0) - *this;
		};

		Tuple operator*(const T scalar) const
		{
			return Tuple(this->m_x * scalar, this->m_y * scalar, this->m_z * scalar, this->m_w * scalar);
		};

		Tuple& operator*=(const T scalar)
		{
			Tuple res = *this * scalar;
			this->m_x = res.m_x;
			this->m_y = res.m_y;
			this->m_z = res.m_z;
			this->m_w = res.m_w;
			return *this;
		}

		Tuple operator/(const T scalar) const
		{
			return Tuple(this->m_x / scalar, this->m_y / scalar, this->m_z / scalar, this->m_w / scalar);
		};

		Tuple& operator/=(const T scalar)
		{
			Tuple res = *this / scalar;
			this->m_x = res.m_x;
			this->m_y = res.m_y;
			this->m_z = res.m_z;
			this->m_w = res.m_w;
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& os, const Tuple& tuple)
		{
			os << tuple.x() << ", " << tuple.y() << ", " << tuple.z() << ", " << tuple.w();
			return os;
		};
	protected:
		T m_x, m_y, m_z, m_w;
	};
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
