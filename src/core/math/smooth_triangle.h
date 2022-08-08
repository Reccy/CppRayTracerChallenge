#pragma once
#include "triangle.h"

namespace CppRayTracerChallenge::Core::Math
{
	class SmoothTriangle : public Triangle
	{
	public:
		SmoothTriangle() = delete;

		SmoothTriangle(Point p1, Point p2, Point p3, Vector n1, Vector n2, Vector n3);

		const Vector normalLocal(const double u, const double v) const;

		const Vector normal(const double u, const double v) const;

		Vector n1() const;

		Vector n2() const;

		Vector n3() const;
	protected:
		Vector m_n1, m_n2, m_n3;
	};
}
