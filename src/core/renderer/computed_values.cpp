#include "computed_values.h"
#include "shape.h"
#include "../math/point.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

ComputedValues::ComputedValues(Math::Intersection intersection, Math::Ray ray) : m_t(intersection.t()), m_shape(static_cast<const Renderer::Shape&>(intersection.shape()))
{
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
}

double ComputedValues::t() const
{
	return m_t;
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
