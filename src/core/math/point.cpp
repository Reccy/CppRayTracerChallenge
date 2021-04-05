#include "point.h"

using namespace CppRayTracerChallenge::Core::Math;

double Point::x() const
{
	return this->m_tuple.x();
}

double Point::y() const
{
	return this->m_tuple.y();
}

double Point::z() const
{
	return this->m_tuple.z();
}

double Point::w() const
{
	return this->m_tuple.w();
}

bool Point::operator==(const Point& other) const
{
	return this->m_tuple == other.m_tuple;
}

bool Point::operator!=(const Point& other) const
{
	return !(*this == other);
}

Point Point::operator+(const Point& other) const
{
	return Point(this->m_tuple + other.m_tuple);
}

Point Point::operator+(const Vector& other) const
{
	return Point(this->m_tuple + other.m_tuple);
}

Vector Point::operator-(const Point& other) const
{
	return Vector(this->m_tuple - other.m_tuple);
}

Point Point::operator-(const Vector& other) const
{
	return Point(this->m_tuple - other.m_tuple);
}

Point Point::operator-() const
{
	return Point(Tuple(0,0,0,0) - this->m_tuple);
}

Point Point::operator*(const double scalar) const
{
	return Point(this->m_tuple * scalar);
}

Point Point::operator/(const double scalar) const
{
	return Point(this->m_tuple / scalar);
}

std::ostream& CppRayTracerChallenge::Core::Math::operator<<(std::ostream& os, const Point& point)
{
	os << point.m_tuple;
	return os;
}
