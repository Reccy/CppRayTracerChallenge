#include "double.h"
#include <cmath>

namespace CppRayTracerChallenge::Core::Math::Double {
	bool compare(const double lhs, const double rhs)
	{
		return abs(lhs - rhs) < EPSILON;
	}
}
