#include "group.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

void Group::transform(Transform transform)
{
	m_transform = transform;
}

const Transform Group::transform() const
{
	return m_transform;
}

const Vector Group::normal(const Point) const
{
	throw std::logic_error("Method not implemented");
}

const Vector Group::normalLocal(const Point) const
{
	throw std::logic_error("Method not implemented");
}

const Intersections Group::intersect(Ray ray) const
{
	ray = ray.transform(m_transform.invert());

	return intersectLocal(ray);
}

const Intersections Group::intersectLocal(Ray ray) const
{
	Intersections result = Intersections();

	for (int i = 0; i < m_shapes.size(); ++i)
	{
		result += m_shapes[i]->intersect(ray);
	}

	return result;
}

void Group::addChild(std::shared_ptr<Shape> child)
{
	m_shapes.push_back(child);
	child->parent(weak_from_this());
}

void Group::addChild(std::shared_ptr<Group> child)
{
	m_groups.push_back(child);
	child->m_parent = weak_from_this();
}

std::weak_ptr<Group> Group::parent() const
{
	return m_parent;
}

const Point Group::worldToObject(Point worldPosition) const
{
	Point result = worldPosition;

	if (!m_parent.expired())
	{
		result = m_parent.lock()->worldToObject(worldPosition);
	}

	return m_transform.invert() * result;
}

const Vector Group::normalToWorld(Vector objectNormal) const
{
	Math::Vector normal = m_transform.invert().transpose() * objectNormal;
	normal = normal.normalize();

	if (!m_parent.expired())
	{
		normal = m_parent.lock()->normalToWorld(normal);
	}

	return normal;
}

bool Group::includes(std::shared_ptr<Shape> child) const
{
	for (int i = 0; i < m_shapes.size(); ++i)
	{
		if (m_shapes[i] == child)
		{
			return true;
		}
	}

	return false;
}

int Group::size() const
{
	return static_cast<int>(m_shapes.size());
}

int Group::count() const
{
	return size();
}

bool Group::empty() const
{
	return size() == 0;
}
