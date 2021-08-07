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

	if (!bounds().intersects(ray))
	{
		return result;
	}
	
	for (int i = 0; i < m_groups.size(); ++i)
	{
		result += m_groups[i]->intersect(ray);
	}
	
	for (int i = 0; i < m_shapes.size(); ++i)
	{
		result += m_shapes[i]->intersect(ray);
	}

	return result;
}

const BoundingBox Group::bounds() const
{
	return m_bounds;
}

void Group::recalculateBounds()
{
	BoundingBox box = BoundingBox();

	for (int i = 0; i < m_groups.size(); ++i)
	{
		box.add(m_groups.at(i)->parentSpaceBounds());
	}

	for (int i = 0; i < m_shapes.size(); ++i)
	{
		box.add(m_shapes.at(i)->parentSpaceBounds());
	}

	m_bounds = box;
}

const BoundingBox Group::parentSpaceBounds() const
{
	BoundingBox result = BoundingBox(bounds());
	result.transform(m_transform);
	return result;
}

void Group::addObject(std::shared_ptr<Shape> child)
{
	return addChild(child);
}

void Group::addObject(std::shared_ptr<Group> child)
{
	return addChild(child);
}

void Group::addChild(std::shared_ptr<Shape> child)
{
	m_shapes.push_back(child);
	child->parent(weak_from_this());

	recalculateBounds();
}

void Group::addChild(std::shared_ptr<Group> child)
{
	m_groups.push_back(child);
	child->m_parent = weak_from_this();

	recalculateBounds();
}

void Group::makeSubgroup(std::vector<std::shared_ptr<Shape>> children)
{
	auto subgroup = std::make_shared<Group>();
	
	for (std::shared_ptr<Shape> ptr : children)
	{
		subgroup->addChild(ptr);
		ptr->parent(weak_from_this());
	}

	m_groups.push_back(subgroup);
	recalculateBounds();
}

std::shared_ptr<Group> Group::subgroup(int index) const
{
	return m_groups.at(index);
}

std::shared_ptr<Shape> Group::child(int index) const
{
	return m_shapes.at(index);
}

void Group::divide(int threshold)
{
	if (threshold <= m_shapes.size())
	{
		auto [left, right] = partitionChildren();

		if (!left.empty())
		{
			makeSubgroup(left);
		}

		if (!right.empty())
		{
			makeSubgroup(right);
		}
	}

	for (std::shared_ptr<Group> subgroup : m_groups)
	{
		subgroup->divide(threshold);
	}
}

std::weak_ptr<Group> Group::parent() const
{
	return m_parent;
}

std::tuple<std::vector<std::shared_ptr<Shape>>, std::vector<std::shared_ptr<Shape>>> Group::partitionChildren()
{
	auto [leftBox, rightBox] = bounds().split();

	std::vector<std::shared_ptr<Shape>> leftBucket;
	std::vector<std::shared_ptr<Shape>> rightBucket;
	std::vector<std::shared_ptr<Shape>> thisBucket;

	for (int i = 0; i < m_shapes.size(); ++i)
	{
		std::shared_ptr<Shape> shape = m_shapes[i];
		const BoundingBox& shapeBounds = shape->parentSpaceBounds();

		if (leftBox.contains(shapeBounds))
		{
			leftBucket.push_back(shape);
			continue;
		}

		if (rightBox.contains(shapeBounds))
		{
			rightBucket.push_back(shape);
			continue;
		}

		thisBucket.push_back(shape);
	}

	m_shapes = thisBucket;

	return std::tuple(leftBucket, rightBucket);
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

bool Group::includes(const IShape& other) const
{
	if (auto shape = dynamic_cast<const Shape*>(&other))
	{
		const auto ptr = std::shared_ptr<const Shape>(shape);

		return includes(ptr);
	}

	return false;
}

bool Group::includes(std::shared_ptr<const Shape> child) const
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
	return static_cast<int>(m_shapes.size() + m_groups.size());
}

int Group::count() const
{
	return size();
}

bool Group::empty() const
{
	return size() == 0;
}
