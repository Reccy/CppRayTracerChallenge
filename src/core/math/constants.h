#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_CONSTANTS
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_CONSTANTS

#include <numeric>

namespace CppRayTracerChallenge::Core::Math
{
	constexpr float EPSILON = 0.00001f;

	/// <summary>
	/// Positive Infinity
	/// </summary>
	constexpr double INF = std::numeric_limits<double>::infinity();
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_CONSTANTS
