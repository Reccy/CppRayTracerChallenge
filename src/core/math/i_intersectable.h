#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_I_INTERSECTABLE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_I_INTERSECTABLE

#include "point.h"

namespace CppRayTracerChallenge::Core::Math
{
	class IIntersectable
	{
	public:
		virtual Point position() const = 0;
		virtual const Vector normal(const Point position) const = 0;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_I_INTERSECTABLE
