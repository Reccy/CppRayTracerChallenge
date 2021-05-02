#include "intersection.h"

using namespace CppRayTracerChallenge::Core::Math;

Intersection::Intersection(const double t, const IIntersectable& intersectable) : m_t(t), m_intersectable(intersectable) {};

const double Intersection::t() const
{
	return m_t;
}

const IIntersectable& Intersection::intersectable() const
{
	return m_intersectable;
}
