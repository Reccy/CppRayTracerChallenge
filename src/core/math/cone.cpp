#include "cone.h"

using namespace CppRayTracerChallenge::Core::Math;

Cone::Cone()
	: m_minimum(-INFINITY), m_maximum(INFINITY), m_closed(false) {};

Cone::Cone(double minimum, double maximum)
	: m_minimum(minimum), m_maximum(maximum), m_closed(false) {};

Cone::Cone(double minimum, double maximum, bool closed)
	: m_minimum(minimum), m_maximum(maximum), m_closed(closed) {};

const Vector Cone::normalLocal(const Point) const
{
	return Vector();
}

const Intersections Cone::intersectLocal(Ray) const
{
	return Intersections();
}

double Cone::maximum() const
{
	return m_maximum;
}

void Cone::maximum(double maximum)
{
	m_maximum = maximum;
}

double Cone::minimum() const
{
	return m_minimum;
}

void Cone::minimum(double minimum)
{
	m_minimum = minimum;
}

bool Cone::closed() const
{
	return m_closed;
}

void Cone::closed(bool closed)
{
	m_closed = closed;
}
