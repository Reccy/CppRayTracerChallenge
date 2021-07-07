#include "computed_values.h"
#include "shape.h"
#include "../math/point.h"

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

void ComputedValues::calculateValues(Math::Intersection& hit, Math::Ray& ray)
{
	// Standard Values

	m_position = ray.position(m_t);
	m_eye = -ray.direction();
	m_normal = m_shape.normal(m_position);
	m_reflect = Math::Vector::reflect(ray.direction(), m_normal);

	if (Vector::dot(m_normal, m_eye) < 0)
	{
		m_isInside = true;
		m_normal = -m_normal;
	}
	else
	{
		m_isInside = false;
	}

	m_overPosition = m_position + m_normal * Math::EPSILON;

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

const Shape& ComputedValues::shape() const
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
