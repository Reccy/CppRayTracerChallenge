#pragma once
#include "RML.h"
#include "material.h"
#include "i_group.h"
#include "../math/bounding_box.h"
#include "../math/i_shape.h"
#include "../math/transform.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	using Math::Transform;

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

		template <typename T>
		static std::shared_ptr<Shape> build()
		{
			auto t = std::make_shared<T>();
			return std::make_shared<Shape>(t);
		}

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
		std::weak_ptr<IGroup> parent() const;

		/// <summary>
		/// Returns a pointer to the shape encapsulated by the Renderer::Shape
		/// </summary>
		std::shared_ptr<Math::IShape> shape() const;

		/// <summary>
		/// Sets the parent group
		/// </summary>
		/// <param name="parent">Parent group to set</param>
		void parent(std::weak_ptr<IGroup> parent);

		/// <summary>
		/// Transforms a position from world space to object space
		/// </summary>
		/// <param name="worldPoint">The position in world space</param>
		/// <returns>Transformed position in object space</returns>
		const RML::Point worldToObject(RML::Point worldPosition) const;

		/// <summary>
		/// Transforms a normal from object space to world space
		/// </summary>
		/// <param name="objectNormal">The normal in object space</param>
		/// <returns>Transformed normal in world space</returns>
		const RML::Vector normalToWorld(RML::Vector objectNormal) const;

		/// <summary>
		/// Returns the color of the shape at the world position, taking into account the material and lighting
		/// </summary>
		/// <param name="worldPosition">The position in the world of the surface of the shape to render</param>
		/// <returns>The color of the fragment</returns>
		Graphics::Color colorAt(RML::Point worldPosition) const;

		// Delegating methods

		void transform(Transform transform) override;
		Transform& transform() override;
		const RML::Vector normal(const RML::Point position) const override;
		const RML::Vector normalLocal(const RML::Point position) const override;
		const Math::Intersections intersect(Math::Ray ray) const override;
		const Math::Intersections intersectLocal(Math::Ray ray) const override;
		const Math::BoundingBox bounds() const override;
		const Math::BoundingBox parentSpaceBounds() const override;

		// Equality methods

		bool operator==(const Shape& other) const;
		bool operator!=(const Shape& other) const;
	private:
		std::shared_ptr<IShape> m_shape;
		std::weak_ptr<IGroup> m_parent;
		Material m_material;
	};
}
