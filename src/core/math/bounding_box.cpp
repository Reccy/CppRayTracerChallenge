#include "bounding_box.h"
#include "transform.h"
#include "constants.h"

using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core;

BoundingBox::BoundingBox() :
	m_initialMin({ INF, INF, INF }), m_initialMax({ -INF, -INF, -INF }), m_min(m_initialMin), m_max(m_initialMax) {};

BoundingBox::BoundingBox(Point min, Point max) :
	m_initialMin(min), m_initialMax(max), m_min(m_initialMin), m_max(m_initialMax) {};

BoundingBox::CheckAxisResult BoundingBox::checkAxis(double origin, double direction, double min, double max) const
{
	double tMinNumerator = (min - origin);
	double tMaxNumerator = (max - origin);

	double tMin, tMax;

	if (abs(direction) >= EPSILON)
	{
		tMin = tMinNumerator / direction;
		tMax = tMaxNumerator / direction;
	}
	else
	{
		tMin = tMinNumerator * INFINITY;
		tMax = tMaxNumerator * INFINITY;
	}

	BoundingBox::CheckAxisResult result;

	if (tMin > tMax)
	{
		result.tMin = tMax;
		result.tMax = tMin;
	}
	else
	{
		result.tMin = tMin;
		result.tMax = tMax;
	}

	return result;
}

bool BoundingBox::intersects(Ray ray) const
{
	CheckAxisResult x = checkAxis(ray.origin().x(), ray.direction().x(), m_min.x(), m_max.x());
	CheckAxisResult y = checkAxis(ray.origin().y(), ray.direction().y(), m_min.y(), m_max.y());
	CheckAxisResult z = checkAxis(ray.origin().z(), ray.direction().z(), m_min.z(), m_max.z());

	double tMin = std::max({ x.tMin, y.tMin, z.tMin });
	double tMax = std::min({ x.tMax, y.tMax, z.tMax });

	if (tMin > tMax)
	{
		return false;
	}

	if (tMax < 0)
	{
		return false;
	}

	return true;
}

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

std::tuple<BoundingBox, BoundingBox> BoundingBox::split() const
{
	double xSize = m_max.x() - m_min.x();
	double ySize = m_max.y() - m_min.y();
	double zSize = m_max.z() - m_min.z();

	double greatest = std::max({ xSize, ySize, zSize });

	double x0 = m_min.x();
	double y0 = m_min.y();
	double z0 = m_min.z();
	double x1 = m_max.x();
	double y1 = m_max.y();
	double z1 = m_max.z();

	if (greatest == xSize)
	{
		x0 = x1 = x0 + xSize / 2.0;
	}
	else if (greatest == ySize)
	{
		y0 = y1 = y0 + ySize / 2.0;
	}
	else
	{
		z0 = z1 = z0 + zSize / 2.0;
	}

	Point midMin = Point(x0, y0, z0);
	Point midMax = Point(x1, y1, z1);

	BoundingBox left = BoundingBox(m_min, midMax);
	BoundingBox right = BoundingBox(midMin, m_max);

	return std::tuple(left, right);
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
