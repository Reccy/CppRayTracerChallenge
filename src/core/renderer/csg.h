#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CSG
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CSG

#include "../math/i_shape.h"
#include "shape.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class CSG : public Math::IShape, public std::enable_shared_from_this<Group>
	{
	public:
		/// <summary>
		/// Opereration that a CSG takes on its left and right child shapes
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

		void transform(Math::Transform transform) override;

		const Math::Transform transform() const override;

		const Math::Vector normal(const Math::Point position) const override;

		const Math::Vector normalLocal(const Math::Point position) const override;

		const Math::Intersections intersect(Math::Ray ray) const override;

		const Math::Intersections intersectLocal(Math::Ray ray) const override;

		const Math::BoundingBox bounds() const override;

		const Math::BoundingBox parentSpaceBounds() const override;

		CSG(Operation op, std::shared_ptr<Shape> left, std::shared_ptr<Shape> right);

		Operation operation() const;

		std::shared_ptr<Shape> left() const;
		std::shared_ptr<Shape> right() const;
	private:
		Math::Transform m_transform;
		std::shared_ptr<Shape> m_left, m_right;
		Operation m_operation;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CSG
