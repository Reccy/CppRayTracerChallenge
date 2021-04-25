#include "vector.h"

using namespace CppRayTracerChallenge::Core::Math;

Vector::Vector(const Tuple tuple) : Tuple(tuple.x(), tuple.y(), tuple.z(), 0) {};

Vector::Vector(const double x, const double y, const double z) : Tuple(x, y, z, 0) {};

Vector Vector::zero()
{
	return Vector(0, 0, 0);
}

Vector Vector::cross(const Vector& a, const Vector& b)
{
	return Vector (
		a.m_y * b.m_z - a.m_z * b.m_y,
		a.m_z * b.m_x - a.m_x * b.m_z,
		a.m_x * b.m_y - a.m_y * b.m_x
	);
}

double Vector::dot(const Vector& a, const Vector& b)
{
	return a.m_x * b.m_x +
		a.m_y * b.m_y +
		a.m_z * b.m_z;
}

double Vector::magnitude() const
{
	return sqrt(
		pow(this->m_x, 2) +
		pow(this->m_y, 2) +
		pow(this->m_z, 2)
	);
}

Vector Vector::normalize() const
{
	return Vector(*this / this->magnitude());
}
