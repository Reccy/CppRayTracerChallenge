#include "point.h"

using namespace CppRayTracerChallenge::Core::Math;

Point::Point(const Tuple tuple) : Tuple(tuple.x(), tuple.y(), tuple.z(), 1) {};

Point::Point(const double x, const double y, const double z) : Tuple(x, y, z, 1) {};

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
