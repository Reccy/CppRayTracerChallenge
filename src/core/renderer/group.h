#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP

#include "../math/i_shape.h"
#include "../math/transform.h"
#include "../math/intersections.h"
#include "shape.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class Shape;

	class Group : public Math::IShape, public std::enable_shared_from_this<Group>
	{
	public:
		void transform(Math::Transform transform) override;

		const Math::Transform transform() const override;

		const Math::Vector normal(const Math::Point position) const override;

		const Math::Vector normalLocal(const Math::Point position) const override;

		const Math::Intersections intersect(Math::Ray ray) const override;

		const Math::Intersections intersectLocal(Math::Ray ray) const override;

		void addChild(std::shared_ptr<Shape> child);

		bool includes(std::shared_ptr<Shape> child) const;

		int size() const;

		int count() const;

		bool empty() const;
	private:
		Math::Transform m_transform;
		std::vector<std::shared_ptr<Shape>> m_shapes;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP
