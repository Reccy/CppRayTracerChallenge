#include "bounding_box.h"
#include "transform.h"
#include "constants.h"

using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core;

BoundingBox::BoundingBox() :
	m_initialMin({ INF, INF, INF }), m_initialMax({ -INF, -INF, -INF }), m_min(m_initialMin), m_max(m_initialMax) {};

BoundingBox::BoundingBox(Point min, Point max) :
	m_initialMin(min), m_initialMax(max), m_min(m_initialMin), m_max(m_initialMax) {};

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

bool BoundingBox::contains(const Point& position) const
{
	return position.x() >= m_min.x() && position.x() <= m_max.x() &&
		position.y() >= m_min.y() && position.y() <= m_max.y() &&
		position.z() >= m_min.z() && position.z() <= m_max.z();
}

bool BoundingBox::contains(const BoundingBox& other) const
{
	return contains(other.min()) && contains(other.max());
}

void BoundingBox::transform(Transform transform)
{
	m_transform = transform;
	
	m_min = Point(INF, INF, INF);
	m_max = Point(-INF, -INF, -INF);

	Point min = m_initialMin;
	Point max = m_initialMax;

	add(m_transform * min);
	add(m_transform * Point(min.x(), min.y(), max.z()));
	add(m_transform * Point(min.x(), max.y(), min.z()));
	add(m_transform * Point(min.x(), max.y(), max.z()));
	add(m_transform * Point(max.x(), min.y(), min.z()));
	add(m_transform * Point(max.x(), min.y(), max.z()));
	add(m_transform * Point(max.x(), max.y(), min.z()));
	add(m_transform * max);
}

const Transform BoundingBox::transform() const
{
	return m_transform;
}
