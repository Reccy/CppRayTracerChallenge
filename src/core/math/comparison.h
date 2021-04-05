#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_COMPARISON
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_COMPARISON

#include <type_traits>

/// <summary>
/// Double point precision floating point helper functions
/// </summary>
namespace CppRayTracerChallenge::Core::Math::Comparison {
	/// <summary>
	/// Compares two numbers, and return true if they are close enough to EPSILON.
	/// </summary>
	/// <param name="lhs">Left hand side argument</param>
	/// <param name="rhs">Right hand side argument</param>
	/// <returns>True if equal (within EPSILON), False otherwise</returns>
	template<typename T>
	bool equal(const T lhs, const T rhs)
	{
		if (constexpr(std::is_floating_point<T>::value))
		{
			return abs(lhs - rhs) < 0.00001f; // EPSILON comparison
		}
		else
		{
			return lhs == rhs;
		}
	};
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_COMPARISON
