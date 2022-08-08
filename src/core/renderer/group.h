#pragma once
#include "RML.h"
#include "../math/i_shape.h"
#include "../math/intersections.h"
#include "shape.h"
#include "i_group.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class Shape;

	class Group : public Math::IShape, public IGroup, public std::enable_shared_from_this<Group>
	{
	public:
		void transform(Transform transform) override;

		Transform& transform() override;

		const RML::Vector normal(const RML::Point position) const override;

		const RML::Vector normalLocal(const RML::Point position) const override;

		const Math::Intersections intersect(Math::Ray ray) const override;

		const Math::Intersections intersectLocal(Math::Ray ray) const override;

		const Math::BoundingBox bounds() const override;

		const Math::BoundingBox parentSpaceBounds() const override;

		// alias of addChild
		void addObject(std::shared_ptr<Shape> child);

		// alias of addChild
		void addObject(std::shared_ptr<Group> child);

		void addChild(std::shared_ptr<Shape> child);

		void addChild(std::shared_ptr<Group> child);

		void makeSubgroup(std::vector<std::shared_ptr<Shape>> children);

		std::shared_ptr<Group> subgroup(int index) const;

		std::shared_ptr<Shape> child(int index) const;

		void divide(int threshold);

		std::weak_ptr<Group> parent() const;

		std::tuple<std::vector<std::shared_ptr<Shape>>, std::vector<std::shared_ptr<Shape>>> partitionChildren();

		const RML::Point worldToObject(RML::Point worldPosition) const override;

		const RML::Vector normalToWorld(RML::Vector objectNormal) const override;

		bool includes(const IShape& other) const override;

		bool includes(std::shared_ptr<const Shape> child) const;

		int size() const;

		int count() const;

		bool empty() const;
	private:
		Transform m_transform;
		Math::BoundingBox m_bounds;
		std::weak_ptr<Group> m_parent;
		std::vector<std::shared_ptr<Shape>> m_shapes;
		std::vector<std::shared_ptr<Group>> m_groups;

		void recalculateBounds();
	};
}
