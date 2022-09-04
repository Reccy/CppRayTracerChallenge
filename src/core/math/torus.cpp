#include <math.h>
#include "RML.h"
#include "torus.h"
#include "comparison.h"

using namespace CppRayTracerChallenge::Core::Math;

static double BIG_EPSILON = 1e-9;

// Solver Code Adapted from: https://github.com/marcin-chwedczuk/ray_tracing_torus_js/blob/master/app/scripts/solver.js
std::vector<double> SolvePolynomial2(double c0, double c1, double c2)
{
	double p = c1 / (2 * c2);
	double q = c0 / c2;

	double d = p * p - q;

	if (Comparison::equal(d, 0.0, BIG_EPSILON))
	{
		return { -p };
	}
	else if (d < 0)
	{
		return {};
	}

	double sqrtD = sqrt(d);
	return { sqrtD - p, -sqrtD - p };
}

std::vector<double> SolvePolynomial3(double c0, double c1, double c2, double c3)
{
	double a = c2 / c3;
	double b = c1 / c3;
	double c = c0 / c3;

	double sqA = a * a;
	double p = 1.0 / 3.0 * (-1.0 / 3.0 * sqA + b);
	double q = 1.0 / 2.0 * (2.0 / 27.0 * a * sqA - 1.0 / 3.0 * a * b + c);

	double cbP = p * p * p;
	double d = q * q + cbP;

	std::vector<double> result;

	if (Comparison::equal(d, 0.0, BIG_EPSILON))
	{
		if (Comparison::equal(q, 0.0, BIG_EPSILON))
		{
			result = { 0 };
		}
		else
		{
			double u = cbrt(-q);
			result = { 2 * u, -u };
		}
	}
	else if (d < 0)
	{
		double phi = 1.0 / 3.0 * acos(-q / sqrt(-cbP));
		double t = 2 * sqrt(-p);

		result = {
			t * cos(phi),
			-t * cos(phi + RML::Trig::PI / 3.0),
			-t * cos(phi - RML::Trig::PI / 3.0)
		};
	}
	else
	{
		double sqrtD = sqrt(d);
		double u = cbrt(sqrtD - q);
		double v = -cbrt(sqrtD + q);

		result = { u + v };
	}

	double sub = 1.0 / 3.0 * a;
	
	for (size_t i = 0; i < result.size(); i++)
	{
		result[i] -= sub;
	}

	return result;
}

std::vector<double> SolvePolynomial4(double c0, double c1, double c2, double c3, double c4)
{
	double a = c3 / c4;
	double b = c2 / c4;
	double c = c1 / c4;
	double d = c0 / c4;

	double sqA = a * a;
	double p = -3.0 / 8.0 * sqA + b;
	double q = 1.0 / 8.0 * sqA * a - 1.0 / 2.0 * a * b + c;
	double r = -3.0 / 256.0 * sqA * sqA + 1.0 / 16.0 * sqA * b - 1.0 / 4.0 * a * c + d;
	
	std::vector<double> result;

	if (Comparison::equal(r, 0.0, BIG_EPSILON))
	{
		result = SolvePolynomial3(q, p, 0, 1);
		result.push_back(0);
	}
	else
	{
		result = SolvePolynomial3(
			1.0 / 2.0 * r * p - 1.0 / 8.0 * q * q,
			-r,
			-1.0 / 2.0 * p,
			1.0
		);

		double z = result[0];

		double u = z * z - r;
		double v = 2 * z - p;

		if (Comparison::equal(u, 0.0, BIG_EPSILON))
		{
			u = 0;
		}
		else if (u > 0)
		{
			u = sqrt(u);
		}
		else
		{
			return { 0 };
		}

		if (Comparison::equal(v, 0.0, BIG_EPSILON))
		{
			v = 0;
		}
		else if (v > 0)
		{
			v = sqrt(v);
		}
		else
		{
			return { 0 };
		}

		result = SolvePolynomial2(
			z - u,
			q < 0 ? -v : v,
			1
		);

		std::vector<double> resultAppend = SolvePolynomial2(
			z + u,
			q < 0 ? v : -v,
			1
		);

		result.insert(result.end(), resultAppend.begin(), resultAppend.end());
	}

	double sub = 1.0 / 4.0 * a;

	for (size_t i = 0; i < result.size(); i++)
	{
		result[i] -= sub;
	}

	return result;
}

// Intersection Code Adapted from: https://github.com/marcin-chwedczuk/ray_tracing_torus_js/blob/master/app/scripts/Torus.js
namespace CppRayTracerChallenge::Core::Math
{
	Torus::Torus() :
		m_tubeRadius(1),
		m_sweepRadius(1)
	{};

	Torus::Torus(double tubeRadius, double sweepRadius) :
		m_tubeRadius(tubeRadius),
		m_sweepRadius(sweepRadius)
	{};

	const Vector Torus::normalLocal(const Point position) const
	{
		double paramSquared = m_sweepRadius * m_sweepRadius + m_tubeRadius * m_tubeRadius;

		double x = position.x();
		double y = position.y();
		double z = position.z();

		double sumSquared = x * x + y * y + z * z;

		Vector result(
			4.0 * x * (sumSquared - paramSquared),
			4.0 * y * (sumSquared - paramSquared + 2.0 * m_sweepRadius * m_sweepRadius),
			4.0 * z * (sumSquared - paramSquared)
		);

		return result.normalized();
	}

	const Intersections Torus::intersectLocal(Ray ray) const
	{
		double ox = ray.origin().x();
		double oy = ray.origin().y();
		double oz = ray.origin().z();

		double dx = ray.direction().x();
		double dy = ray.direction().y();
		double dz = ray.direction().z();

		double sumDSqrd = dx * dx + dy * dy + dz * dz;
		double e = ox * ox + oy * oy + oz * oz - m_sweepRadius * m_sweepRadius - m_tubeRadius * m_tubeRadius;
		double f = ox * dx + oy * dy + oz * dz;
		double fourASqrd = 4.0 * m_sweepRadius * m_sweepRadius;

		std::vector<double> solutions = SolvePolynomial4
		(
			e * e - fourASqrd * (m_tubeRadius * m_tubeRadius - oy * oy),
			4.0 * f * e + 2.0 * fourASqrd * oy * dy,
			2.0 * sumDSqrd * e + 4.0 * f * f + fourASqrd * dy * dy,
			4.0 * sumDSqrd * f,
			sumDSqrd * sumDSqrd
		);

		if (solutions.size() == 0)
		{
			return Intersections();
		}

		std::vector<Intersection> intersections;
		for (double t : solutions)
		{
			if (std::isinf(t) || std::isnan(t) || t <= RML::EPSILON)
			{
				continue;
			}

			intersections.push_back(Intersection(t, *this));
		}

		return Intersections(intersections);
	}

	const BoundingBox Torus::bounds() const
	{
		return BoundingBox();
	}
}
