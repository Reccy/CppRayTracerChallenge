#include "bounding_box.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

BoundingBox::BoundingBox() :
	m_min({ INFINITY, INFINITY, INFINITY }), m_max({ -INFINITY, -INFINITY, -INFINITY }) {};

BoundingBox::BoundingBox(Math::Point min, Math::Point max) :
	m_min(min), m_max(max) {};

Math::Point BoundingBox::min() const
{
	return m_min;
}

Math::Point BoundingBox::max() const
{
	return m_max;
}
