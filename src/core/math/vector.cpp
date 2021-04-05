#include "vector.h"

using namespace CppRayTracerChallenge::Core::Math;

Vector Vector::zero()
{
	return Vector(0, 0, 0);
}

double Vector::x() const
{
	return this->m_tuple.x();
}

double Vector::y() const
{
	return this->m_tuple.y();
}

double Vector::z() const
{
	return this->m_tuple.z();
}

double Vector::w() const
{
	return this->m_tuple.w();
}

Vector Vector::cross(const Vector a, const Vector b)
{
	return Vector (
		a.m_tuple.y() * b.m_tuple.z() - a.m_tuple.z() * b.m_tuple.y(),
		a.m_tuple.z() * b.m_tuple.x() - a.m_tuple.x() * b.m_tuple.z(),
		a.m_tuple.x() * b.m_tuple.y() - a.m_tuple.y() * b.m_tuple.x()
	);
}

double Vector::dot(const Vector a, const Vector b)
{
	return a.m_tuple.x() * b.m_tuple.x() +
		a.m_tuple.y() * b.m_tuple.y() +
		a.m_tuple.z() * b.m_tuple.z();
}

double Vector::magnitude() const
{
	return sqrt(
		pow(this->m_tuple.x(), 2) +
		pow(this->m_tuple.y(), 2) +
		pow(this->m_tuple.z(), 2)
	);
}

Vector Vector::normalize() const
{
	return Vector(this->m_tuple / this->magnitude());
}

bool Vector::operator==(const Vector& other) const
{
	return	other.m_tuple == this->m_tuple;
}

bool Vector::operator!=(const Vector& other) const
{
	return !(*this == other);
}

Vector Vector::operator+(const Vector& other) const
{
	return Vector(this->m_tuple + other.m_tuple);
}

Vector Vector::operator-(const Vector& other) const
{
	return Vector(this->m_tuple - other.m_tuple);
}

Vector Vector::operator-() const
{
	return Vector::zero() - *this;
}

Vector Vector::operator*(const double scalar) const
{
	return Vector(this->m_tuple * scalar);
}

Vector Vector::operator/(const double scalar) const
{
	return Vector(this->m_tuple / scalar);
}

std::ostream& CppRayTracerChallenge::Core::Math::operator<<(std::ostream& os, const Vector& vector)
{
	os << vector.m_tuple;
	return os;
}
