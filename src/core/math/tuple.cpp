#include "tuple.h"
#include "double.h"

using namespace CppRayTracerChallenge::Core::Math;

Tuple Tuple::buildPoint(const double x, const double y, const double z)
{
	return Tuple(x, y, z, 1.0f);
};

Tuple Tuple::zeroVector()
{
	return Tuple(0, 0, 0, 0);
}

double Tuple::x() const
{
	return this->m_x;
}

double Tuple::y() const
{
	return this->m_y;
}

double Tuple::z() const
{
	return this->m_z;
}

double Tuple::w() const
{
	return this->m_w;
}

bool Tuple::operator==(const Tuple& other) const
{
	return	Double::compare(this->m_x, other.m_x) &&
		Double::compare(this->m_y, other.m_y) &&
		Double::compare(this->m_z, other.m_z) &&
		Double::compare(this->m_w, other.m_w);
}

bool Tuple::operator!=(const Tuple& other) const
{
	return !(*this == other);
}

Tuple Tuple::operator+(const Tuple& other) const
{
	return Tuple(this->m_x + other.m_x, this->m_y + other.m_y, this->m_z + other.m_z, this->m_w + other.m_w);
}

Tuple Tuple::operator-(const Tuple& other) const
{
	return Tuple(this->m_x - other.m_x, this->m_y - other.m_y, this->m_z - other.m_z, this->m_w - other.m_w);
}

Tuple Tuple::operator-() const
{
	return Tuple::zeroVector() - *this;
}

Tuple Tuple::operator*(const double scalar) const
{
	return Tuple(this->m_x * scalar, this->m_y * scalar, this->m_z * scalar, this->m_w * scalar);
}

Tuple Tuple::operator/(const double scalar) const
{
	return Tuple(this->m_x / scalar, this->m_y / scalar, this->m_z / scalar, this->m_w / scalar);
}

std::ostream& CppRayTracerChallenge::Core::Math::operator<<(std::ostream& os, const Tuple& tuple)
{
	os << tuple.x() << ", " << tuple.y() << ", " << tuple.z() << ", " << tuple.w();
	return os;
}
