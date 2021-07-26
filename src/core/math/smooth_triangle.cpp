#include "smooth_triangle.h"

using namespace CppRayTracerChallenge::Core::Math;

SmoothTriangle::SmoothTriangle(Point p1, Point p2, Point p3, Vector n1, Vector n2, Vector n3)
	: Triangle(p1, p2, p3), m_n1(n1), m_n2(n2), m_n3(n3) {};

Vector SmoothTriangle::n1() const
{
	return m_n1;
}

Vector SmoothTriangle::n2() const
{
	return m_n2;
}

Vector SmoothTriangle::n3() const
{
	return m_n3;
}
