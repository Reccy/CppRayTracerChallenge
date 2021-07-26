#include "intersection.h"

using namespace CppRayTracerChallenge::Core::Math;

Intersection::Intersection(const double t, const IShape& shape)
	: m_t(t), m_shape(shape), m_u(0), m_v(0) {};

Intersection::Intersection(const double t, const IShape& shape, const double u, const double v)
	: m_t(t), m_shape(shape), m_u(u), m_v(v) {};

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

std::ostream& CppRayTracerChallenge::Core::Math::operator<<(std::ostream& os, const Intersection& intersection)
{
	os << "t(" << intersection.t() << "), shape_ptr(" << &intersection.shape() << "), u(" << intersection.u() << "), v(" << intersection.v() << ")";
	return os;
}
