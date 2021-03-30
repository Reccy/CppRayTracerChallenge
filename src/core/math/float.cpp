#include "float.h"
#include <cmath>

namespace CppRayTracerChallenge::Core::Math::Float {
	bool compare(const float lhs, const float rhs)
	{
		return abs(lhs - rhs) < EPSILON;
	}

	float sqrt(const float value)
	{
		return (float)::sqrt(value);
	}

	float pow(const float base, const float power)
	{
		return (float)::pow(base, power);
	}
}
