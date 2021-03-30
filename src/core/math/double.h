#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_DOUBLE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_DOUBLE

/// <summary>
/// Double point precision floating point helper functions
/// </summary>
namespace CppRayTracerChallenge::Core::Math::Double {
	/// <summary>
	/// Margin of error for comparing doubles
	/// </summary>
	constexpr double EPSILON = 0.00001;

	/// <summary>
	/// Compares two double numbers, and return true if they are close enough to EPSILON.
	/// </summary>
	/// <param name="lhs">Left hand side argument</param>
	/// <param name="rhs">Right hand side argument</param>
	/// <returns>True if equal (within EPSILON), False otherwise</returns>
	bool compare(const double lhs, const double rhs);
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_DOUBLE
