#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SHAPE
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SHAPE

#include "material.h"
#include "group.h"
#include "../math/transform.h"
#include "../math/vector.h"
#include "../math/i_shape.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class Group;

	/// <summary>
	/// Represents a shape within the rendered world.
	/// This is a wrapper around a Math::IShape which adds materials
	/// </summary>
	class Shape : public Math::IShape
	{
	public:
		Shape() = delete;

		Shape(std::shared_ptr<Math::IShape> shape);

		Shape(std::shared_ptr<Math::IShape>, Material material);

		/// <summary>
		/// Returns the material assigned to the shape
		/// </summary>
		/// <returns>Material assigned to the shape</returns>
		const Material material() const;

		/// <summary>
		/// Assigns a material to the shape
		/// </summary>
		/// <param name="material">Material to assign</param>
		void material(const Material material);

		/// <summary>
		/// Returns the parent group if it exists
		/// </summary>
		/// <returns>Parent</returns>
		std::optional<std::shared_ptr<Group>> parent() const;

		/// <summary>
		/// Returns the color of the shape at the world position, taking into account the material and lighting
		/// </summary>
		/// <param name="worldPosition">The position in the world of the surface of the shape to render</param>
		/// <returns>The color of the fragment</returns>
		Graphics::Color colorAt(Math::Point worldPosition) const;

		// Delegating methods

		void transform(Math::Transform transform) override;
		const Math::Transform transform() const override;
		const Math::Vector normal(const Math::Point position) const override;
		const Math::Vector normalLocal(const Math::Point position) const override;
		const Math::Intersections intersect(Math::Ray ray) const override;
		const Math::Intersections intersectLocal(Math::Ray ray) const override;

		// Equality methods

		bool operator==(const Shape& other) const;
		bool operator!=(const Shape& other) const;
	private:
		std::shared_ptr<IShape> m_shape;
		std::optional<std::shared_ptr<Group>> m_parent;
		Material m_material;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SHAPE
