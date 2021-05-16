#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_I_INTERSECTABLE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_I_INTERSECTABLE

#include "intersections.h"
#include "ray.h"

namespace CppRayTracerChallenge::Core::Math
{
	class Ray;
	class Intersections;
	class IIntersectable
	{
	public:
		virtual const Intersections intersect(const Ray& ray) const = 0;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_I_INTERSECTABLE
