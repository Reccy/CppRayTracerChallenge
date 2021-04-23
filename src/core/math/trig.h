#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_TRIG
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_TRIG

/// <summary>
/// Trigonometry helpers
/// </summary>
namespace CppRayTracerChallenge::Core::Math::Trig
{
	constexpr double PI = 3.141592653589793238462643383279502884;

	/// <summary>
	/// Converts degrees to radians
	/// </summary>
	/// <param name="degrees">The degrees to convert</param>
	/// <returns>The radians that correspond to the input degrees</returns>
	double degrees_to_radians(const double degrees);

	/// <summary>
	/// Converts radians to degrees
	/// </summary>
	/// <param name="radians">The radians to convert</param>
	/// <returns>The degrees that correspond to the input radians</returns>
	double radians_to_degrees(const double radians);
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_TRIG
