#include "intersection.h"

using namespace CppRayTracerChallenge::Core::Math;

Intersection::Intersection(const double t, const Sphere& intersectable) : m_t(t), m_intersectable(intersectable) {}; // NOTE: Temporarily removing IIntersectable. Replace with Shape in future refactor

const double Intersection::t() const
{
	return m_t;
}

const Sphere& Intersection::intersectable() const // NOTE: Temporarily removing IIntersectable. Replace with Shape in future refactor
{
	return m_intersectable;
}

bool Intersection::operator==(const Intersection& other) const
{
	return this->m_t == other.m_t && &this->m_intersectable == &other.m_intersectable;
}

bool Intersection::operator!=(const Intersection& other) const
{
	return !((*this) == other);
}
