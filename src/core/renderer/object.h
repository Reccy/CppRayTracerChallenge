#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_OBJECT
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_OBJECT

#include "../math/sphere.h"
#include "material.h"
#include "../math/intersections.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	/// <summary>
	/// Represents an object within the rendered world.
	/// The Object is composed of a shape and a material
	/// </summary>
	class Object
	{
	public:
		Object(Math::Sphere shape);
		Object(Math::Sphere shape, Material material);

		/// <summary>
		/// Returns the material assigned to the object
		/// </summary>
		/// <returns>Material assigned to the object</returns>
		const Material material() const;

		/// <summary>
		/// Assigns a material to the object
		/// </summary>
		/// <param name="material">Material to assign</param>
		void material(const Material material);

		/// <summary>
		/// Intersects a ray on the shape
		/// </summary>
		/// <param name="ray">The ray to intersect through the shape</param>
		/// <returns>The Intersections between the ray and the shape</returns>
		Math::Intersections intersect(const Math::Ray& ray) const;
	private:
		Math::Sphere m_shape;
		Material m_material;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_OBJECT
