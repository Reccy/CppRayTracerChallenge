#include "computed_values.h"
#include "shape.h"
#include "csg.h"
#include "../math/smooth_triangle.h"
#include "RML.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

ComputedValues::ComputedValues(Math::Intersection intersection, Math::Ray ray) : m_t(intersection.t()), m_n1(0), m_n2(0), m_shape(static_cast<const Renderer::Shape&>(intersection.shape())), m_intersections(std::vector<Math::Intersection> { intersection })
{
	calculateValues(intersection, ray);
	
}

ComputedValues::ComputedValues(Math::Intersection intersection, Math::Ray ray, Math::Intersections intersections) : m_t(intersection.t()), m_n1(0), m_n2(0), m_shape(static_cast<const Renderer::Shape&>(intersection.shape())), m_intersections(intersections)
{
	calculateValues(intersection, ray);
}

int includedIn(const std::vector<CppRayTracerChallenge::Core::Renderer::Shape>& containers, const CppRayTracerChallenge::Core::Renderer::Shape& shape)
{
	for (int i = 0; i < containers.size(); ++i)
	{
		if (containers.at(i) == shape)
			return i;
	}

	return -1;
}

float schlick(const ComputedValues& cv)
{
	float cos = static_cast<float>(Vector::dot(cv.eye(), cv.normal()));

	if (cv.n1() > cv.n2())
	{
		float n = cv.n1() / cv.n2();
		float sin2t = powf(n, 2) * (1.0f - powf(cos, 2));

		if (sin2t > 1.0f)
		{
			return 1.0f;
		}

		float cosT = sqrtf(1.0f - sin2t);

		cos = cosT;
	}

	float r0 = powf((cv.n1() - cv.n2()) / (cv.n1() + cv.n2()), 2);

	return r0 + (1 - r0) * powf(1 - cos, 5);
}

void ComputedValues::calculateValues(Math::Intersection& hit, Math::Ray& ray)
{
	// Standard Values

	m_position = ray.position(m_t);
	m_eye = -ray.direction();

	if (auto tri = dynamic_cast<SmoothTriangle*>(m_shape.shape().get()))
	{
		m_normal = tri->normal(hit.u(), hit.v());
	}
	else
	{
		m_normal = m_shape.normal(m_position);
	}

	m_reflect = RML::Vector::reflect(ray.direction(), m_normal);

	if (Vector::dot(m_normal, m_eye) < 0)
	{
		m_isInside = true;
		m_normal = -m_normal;
	}
	else
	{
		m_isInside = false;
	}

	m_overPosition = m_position + m_normal * RML::EPSILON;
	m_underPosition = m_position + -m_normal * RML::EPSILON;

	// Calculating n1 and n2

	std::vector<Renderer::Shape> containers;

	for (int i = 0; i < m_intersections.size(); ++i)
	{
		const Math::Intersection& intersection = m_intersections.at(i);
		
		bool intersectionIsHit = intersection == hit;

		if (intersectionIsHit)
		{
			if (containers.empty())
			{
				m_n1 = 1.0f;
			}
			else
			{
				m_n1 = containers.at(containers.size() - 1).material().refractiveIndex;
			}
		}

		int includedInIndex = includedIn(containers, static_cast<const Renderer::Shape&>(intersection.shape()));

		if (includedInIndex > -1)
		{
			containers.erase(containers.begin() + includedInIndex);
		}
		else
		{
			containers.push_back(static_cast<const Renderer::Shape&>(intersection.shape()));
		}

		if (intersectionIsHit)
		{
			if (containers.empty())
			{
				m_n2 = 1.0f;
			}
			else
			{
				m_n2 = containers.at(containers.size() - 1).material().refractiveIndex;
			}
		}
	}

	// Calculating reflectance

	m_reflectance = schlick(*this);
}

double ComputedValues::t() const
{
	return m_t;
}

float ComputedValues::n1() const
{
	return m_n1;
}

float ComputedValues::n2() const
{
	return m_n2;
}

float ComputedValues::reflectance() const
{
	return m_reflectance;
}

const CppRayTracerChallenge::Core::Renderer::Shape& ComputedValues::shape() const
{
	return m_shape;
}

Material ComputedValues::material() const
{
	return m_shape.material();
}

Point ComputedValues::position() const
{
	return m_position;
}

Point ComputedValues::overPosition() const
{
	return m_overPosition;
}

Point ComputedValues::underPosition() const
{
	return m_underPosition;
}

Vector ComputedValues::eye() const
{
	return m_eye;
}

Vector ComputedValues::normal() const
{
	return m_normal;
}

Vector ComputedValues::reflect() const
{
	return m_reflect;
}

bool ComputedValues::isInside() const
{
	return m_isInside;
}
