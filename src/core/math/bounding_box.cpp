#include "bounding_box.h"
#include "constants.h"

using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core;

BoundingBox::BoundingBox() :
	m_min({ INF, INF, INF }), m_max({ -INF, -INF, -INF }) {};

BoundingBox::BoundingBox(Point min, Point max) :
	m_min(min), m_max(max) {};

Point BoundingBox::min() const
{
	return m_min;
}

Point BoundingBox::max() const
{
	return m_max;
}

void BoundingBox::add(Point position)
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

	m_min = Point(minX, minY, minZ);
	m_max = Point(maxX, maxY, maxZ);
}

void BoundingBox::add(const BoundingBox& other)
{
	add(other.max());
	add(other.min());
}
