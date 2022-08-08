#include "triangle.h"
#include "RML.h"

namespace CppRayTracerChallenge::Core::Math
{
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

	const Intersections Triangle::intersectLocal(Ray ray) const
	{
		Vector dirCrossE2 = Vector::cross(ray.direction(), m_e2);

		double determinant = Vector::dot(m_e1, dirCrossE2);

		if (abs(determinant) < RML::EPSILON)
		{
			return Intersections();
		}

		double f = 1.0 / determinant;

		Vector p1ToOrigin = ray.origin() - m_p1;
		double u = f * Vector::dot(p1ToOrigin, dirCrossE2);

		if (u < 0 || u > 1)
		{
			return Intersections();
		}

		Vector originCrossE1 = Vector::cross(p1ToOrigin, m_e1);
		double v = f * Vector::dot(ray.direction(), originCrossE1);

		if (v < 0 || (u + v) > 1)
		{
			return Intersections();
		}

		double t = f * Vector::dot(m_e2, originCrossE1);

		return Intersections({ {t, *this, u, v} });
	}

	const BoundingBox Triangle::bounds() const
	{
		BoundingBox box = BoundingBox();
		box.add(p1());
		box.add(p2());
		box.add(p3());

		return box;
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
}
