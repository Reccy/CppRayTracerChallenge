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

	/// <summary>
	/// Calculates the square root of the floating point number
	/// </summary>
	/// <param name="value">The number to perform the square root operation on</param>
	/// <returns>A floating point that is the square root of the param</returns>
	float sqrt(const float value);

	/// <summary>
	/// Calculates the base raised to the power exponent
	/// </summary>
	/// <param name="base">Base value</param>
	/// <param name="exponent">Exponent value</param>
	/// <returns>The result of raising base to the power exponent</returns>
	float pow(const float base, const float exponent);
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_FLOAT
