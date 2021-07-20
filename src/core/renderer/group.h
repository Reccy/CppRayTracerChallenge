#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP

#include "../math/i_shape.h"
#include "../math/transform.h"
#include "../math/intersections.h"
#include "../math/point.h"
#include "shape.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class Shape;

	class Group : public Math::IShape, public std::enable_shared_from_this<Group>
	{
	public:
		void transform(Math::Transform transform);

		const Math::Transform transform() const override;

		const Math::Vector normal(const Math::Point position) const override;

		const Math::Vector normalLocal(const Math::Point position) const override;

		const Math::Intersections intersect(Math::Ray ray) const override;

		const Math::Intersections intersectLocal(Math::Ray ray) const override;

		const Math::BoundingBox bounds() const override;
		
		const Math::BoundingBox parentSpaceBounds() const override;

		void addChild(std::shared_ptr<Shape> child);

		void addChild(std::shared_ptr<Group> child);

		void makeSubgroup(std::vector<std::shared_ptr<Shape>> children);

		std::shared_ptr<Group> subgroup(int index) const;

		std::weak_ptr<Group> parent() const;

		std::tuple<std::vector<std::shared_ptr<Shape>>, std::vector<std::shared_ptr<Shape>>> partitionChildren();

		const Math::Point worldToObject(Math::Point worldPosition) const;

		const Math::Vector normalToWorld(Math::Vector objectNormal) const;

		bool includes(std::shared_ptr<Shape> child) const;

		bool includes(std::shared_ptr<Group> child) const;

		int size() const;

		int count() const;

		bool empty() const;
	private:
		Math::Transform m_transform;
		std::weak_ptr<Group> m_parent;
		std::vector<std::shared_ptr<Shape>> m_shapes;
		std::vector<std::shared_ptr<Group>> m_groups;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP
