#include "point.h"

using namespace CppRayTracerChallenge::Core::Math;

Vector Point::operator-(const Point& other) const
{
	return Vector(Tuple::operator-(other));
}

Point Point::operator-(const Vector& other) const
{
	return Point(Tuple::operator-(other));
}

Tuple<double> Point::operator-() const
{
	return Tuple::operator-();
}
