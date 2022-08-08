#pragma once
#include "../math/i_shape.h"
#include "shape.h"
#include "i_group.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class CSG : public Math::IShape, public IGroup, public std::enable_shared_from_this<CSG>
	{
	public:
		/// <summary>
		/// Operation that a CSG takes on its left and right child shapes
		/// </summary>
		enum class Operation
		{
			/// <summary>
			/// Union combines inputs into a single shape, preserving all external
			/// surfaces.
			/// </summary>
			UNION,
			/// <summary>
			/// Intersection preserves the portion of the inputs that share a volume,
			///  resulting in a single shape with those combined surfaces.
			/// </summary>
			INTERSECT,
			/// <summary>
			/// Difference preserves only the portion of the first shape where it's
			/// not overlapped by the others.
			/// </summary>
			DIFFERENCE
		};

		void transform(Transform transform) override;

		Transform& transform() override;

		const RML::Vector normal(const RML::Point position) const override;

		const RML::Vector normalLocal(const RML::Point position) const override;

		const Math::Intersections intersect(Math::Ray ray) const override;

		const Math::Intersections intersectLocal(Math::Ray ray) const override;

		const Math::BoundingBox bounds() const override;

		const Math::BoundingBox parentSpaceBounds() const override;

		const RML::Point worldToObject(RML::Point worldPosition) const override;

		const RML::Vector normalToWorld(RML::Vector objectNormal) const override;

		bool includes(const IShape& other) const override;

		static std::shared_ptr<CSG> build(Operation op, std::shared_ptr<Shape> left, std::shared_ptr<Shape> right);

		static bool intersectionAllowed(Operation op, bool lhit, bool inl, bool inr);

		Math::Intersections filterIntersections(Math::Intersections intersections) const;

		Operation operation() const;

		std::shared_ptr<Shape> left() const;
		std::shared_ptr<Shape> right() const;

	protected:
		CSG(Operation op);
	private:
		Transform m_transform;
		std::shared_ptr<Shape> m_left, m_right;
		Operation m_operation;
	};
}
