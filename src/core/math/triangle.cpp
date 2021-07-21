#include "triangle.h"
#include "vector.h"

using namespace CppRayTracerChallenge::Core::Math;

Triangle::Triangle(Point p1, Point p2, Point p3) :
	m_p1(p1), m_p2(p2), m_p3(p3), m_e1(p2 - p1), m_e2(p3 - p1), m_normal(Vector::cross(m_e2, m_e1).normalize()) {};

const Vector Triangle::normalLocal(const Point) const
{
	return m_normal;
}

const Vector Triangle::normal() const
{
	return m_normal;
}

const Intersections Triangle::intersectLocal(Ray) const
{
	return Intersections();
}

const BoundingBox Triangle::bounds() const
{
	return BoundingBox();
}

Point Triangle::p1() const
{
	return m_p1;
}

Point Triangle::p2() const
{
	return m_p2;
}

Point Triangle::p3() const
{
	return m_p3;
}

Vector Triangle::e1() const
{
	return m_e1;
}

Vector Triangle::e2() const
{
	return m_e2;
}
