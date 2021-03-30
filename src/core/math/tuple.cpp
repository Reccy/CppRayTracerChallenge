#include "tuple.h"
#include "double.h"

using namespace CppRayTracerChallenge::Core::Math;

Tuple Tuple::buildPoint(const double x, const double y, const double z)
{
	return Tuple(x, y, z, 1.0f);
};

Tuple Tuple::buildVector(const double x, const double y, const double z)
{
	return Tuple(x, y, z, 0.0f);
};

Tuple Tuple::zeroVector()
{
	return Tuple(0, 0, 0, 0);
}

double Tuple::dot(const Tuple a, const Tuple b)
{
	return a.m_x * b.m_x +
		a.m_y * b.m_y +
		a.m_z * b.m_z +
		a.m_w * b.m_w;
}

double Tuple::getX() const
{
	return this->m_x;
}

double Tuple::getY() const
{
	return this->m_y;
}

double Tuple::getZ() const
{
	return this->m_z;
}

double Tuple::getW() const
{
	return this->m_w;
}

bool Tuple::isPoint() const
{
	return this->m_w == 1.0f;
}

bool Tuple::isVector() const
{
	return this->m_w == 0.0f;
}

double Tuple::magnitude() const
{
	return sqrt(
		pow(this->m_x, 2) +
		pow(this->m_y, 2) +
		pow(this->m_z, 2) +
		pow(this->m_w, 2)
	);
}

Tuple Tuple::normalize() const
{
	return Tuple(
		this->m_x / this->magnitude(),
		this->m_y / this->magnitude(),
		this->m_z / this->magnitude(),
		this->m_w / this->magnitude()
	);
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

Tuple Tuple::operator+(const Tuple& other)
{
	return Tuple(this->m_x + other.m_x, this->m_y + other.m_y, this->m_z + other.m_z, this->m_w + other.m_w);
}

Tuple Tuple::operator-(const Tuple& other)
{
	return Tuple(this->m_x - other.m_x, this->m_y - other.m_y, this->m_z - other.m_z, this->m_w - other.m_w);
}

Tuple Tuple::operator-() const
{
	return Tuple::zeroVector() - *this;
}

Tuple Tuple::operator*(const double scalar)
{
	return Tuple(this->m_x * scalar, this->m_y * scalar, this->m_z * scalar, this->m_w * scalar);
}

Tuple Tuple::operator/(const double scalar)
{
	return Tuple(this->m_x / scalar, this->m_y / scalar, this->m_z / scalar, this->m_w / scalar);
}

std::ostream& CppRayTracerChallenge::Core::Math::operator<<(std::ostream& os, const Tuple& tuple)
{
	os << tuple.getX() << ", " << tuple.getY() << ", " << tuple.getZ() << ", " << tuple.getW();
	return os;
}
