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

void BoundingBox::resizeToFit(Math::Point position)
{
	double maxX = m_max.x();
	double maxY = m_max.y();
	double maxZ = m_max.z();
	double minX = m_min.x();
	double minY = m_min.y();
	double minZ = m_min.z();

	if (position.x() < m_min.x())
	{
		minX = position.x();
	}

	if (position.y() < m_min.y())
	{
		minY = position.y();
	}

	if (position.z() < m_min.z())
	{
		minZ = position.z();
	}

	if (position.x() > m_max.x())
	{
		maxX = position.x();
	}

	if (position.y() > m_max.y())
	{
		maxY = position.y();
	}

	if (position.z() > m_max.z())
	{
		maxZ = position.z();
	}

	m_min = Math::Point(minX, minY, minZ);
	m_max = Math::Point(maxX, maxY, maxZ);
}
