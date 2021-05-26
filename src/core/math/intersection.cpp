#include "intersection.h"

using namespace CppRayTracerChallenge::Core::Math;

Intersection::Intersection(const double t, const IShape& shape) : m_t(t), m_shape(shape) {};

const double Intersection::t() const
{
	return m_t;
}

const IShape& Intersection::shape() const
{
	return m_shape;
}

bool Intersection::operator==(const Intersection& other) const
{
	return this->m_t == other.m_t && &this->m_shape == &other.m_shape;
}

bool Intersection::operator!=(const Intersection& other) const
{
	return !((*this) == other);
}
