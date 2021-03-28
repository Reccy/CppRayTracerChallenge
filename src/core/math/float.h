#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_FLOAT
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_FLOAT

/// <summary>
/// Floating point helper functions
/// </summary>
namespace CppRayTracerChallenge::Core::Math::Float {
	/// <summary>
	/// Margin of error for comparing floating points
	/// </summary>
	constexpr float EPSILON = 0.00001f;

	/// <summary>
	/// Compares two floating point numbers, and return true if they are close enough to EPSILON.
	/// </summary>
	/// <param name="lhs">Left hand side argument</param>
	/// <param name="rhs">Right hand side argument</param>
	/// <returns>True if equal (within EPSILON), False otherwise</returns>
	bool compare(const float lhs, const float rhs);
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_FLOAT
