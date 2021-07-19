#include "bounding_box.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

BoundingBox::BoundingBox() :
	m_min({ INFINITY, INFINITY, INFINITY }), m_max({ -INFINITY, -INFINITY, -INFINITY }) {};

Math::Point BoundingBox::min() const
{
	return m_min;
}

Math::Point BoundingBox::max() const
{
	return m_max;
}
