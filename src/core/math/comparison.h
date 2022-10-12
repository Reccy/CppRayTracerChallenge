#pragma once

#include <type_traits>
#include <cmath>
#include "RML.h"

/// <summary>
/// Double point precision floating point helper functions
/// </summary>
namespace CppRayTracerChallenge::Core::Math::Comparison {
	/// <summary>
	/// Compares two numbers, and return true if they are close enough to EPSILON.
	/// </summary>
	/// <param name="lhs">Left hand side argument</param>
	/// <param name="rhs">Right hand side argument</param>
	/// <param name="eps">Custom epsilon argument</param>
	/// <returns>True if equal (within EPSILON), False otherwise</returns>
	template<typename T, typename U>
	bool equal(const T lhs, const T rhs, const U eps)
	{
		if (constexpr(std::is_floating_point<T>::value))
		{
			if (std::isinf((double)lhs) && std::isinf((double)rhs))
			{
				return (lhs == RML::INF && rhs == RML::INF) || (lhs == -RML::INF && rhs == -RML::INF);
			}

			return abs(lhs - rhs) < eps;
		}
		else
		{
			return lhs == rhs;
		}
	};

	/// <summary>
	/// Compares two numbers, and return true if they are close enough to EPSILON.
	/// </summary>
	/// <param name="lhs">Left hand side argument</param>
	/// <param name="rhs">Right hand side argument</param>
	/// <returns>True if equal (within EPSILON), False otherwise</returns>
	template<typename T>
	bool equal(const T lhs, const T rhs)
	{
		return equal(lhs, rhs, RML::EPSILON);
	};
}
