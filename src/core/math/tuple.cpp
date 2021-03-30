#include "tuple.h"
#include "float.h"

using namespace CppRayTracerChallenge::Core::Math;

Tuple Tuple::buildPoint(const float x, const float y, const float z)
{
	return Tuple(x, y, z, 1.0f);
};

Tuple Tuple::buildVector(const float x, const float y, const float z)
{
	return Tuple(x, y, z, 0.0f);
};

Tuple Tuple::zeroVector()
{
	return Tuple(0, 0, 0, 0);
}

float Tuple::getX() const
{
	return this->m_x;
}

float Tuple::getY() const
{
	return this->m_y;
}

float Tuple::getZ() const
{
	return this->m_z;
}

float Tuple::getW() const
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

float Tuple::magnitude() const
{
	return Float::sqrt(
		Float::pow(this->m_x, 2) +
		Float::pow(this->m_y, 2) +
		Float::pow(this->m_z, 2) +
		Float::pow(this->m_w, 2)
	);
}

bool Tuple::operator==(const Tuple& other) const
{
	return	Float::compare(this->m_x, other.m_x) &&
		Float::compare(this->m_y, other.m_y) &&
		Float::compare(this->m_z, other.m_z) &&
		Float::compare(this->m_w, other.m_w);
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

Tuple Tuple::operator*(const float scalar)
{
	return Tuple(this->m_x * scalar, this->m_y * scalar, this->m_z * scalar, this->m_w * scalar);
}

Tuple Tuple::operator/(const float scalar)
{
	return Tuple(this->m_x / scalar, this->m_y / scalar, this->m_z / scalar, this->m_w / scalar);
}

std::ostream& CppRayTracerChallenge::Core::Math::operator<<(std::ostream& os, const Tuple& tuple)
{
	os << tuple.getX() << ", " << tuple.getY() << ", " << tuple.getZ() << ", " << tuple.getW();
	return os;
}
