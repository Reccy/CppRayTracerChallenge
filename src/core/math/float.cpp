#include "float.h"
#include <cmath>

namespace CppRayTracerChallenge::Core::Math::Float {
	bool compare(const float lhs, const float rhs)
	{
		return abs(lhs - rhs) < EPSILON;
	}
}
