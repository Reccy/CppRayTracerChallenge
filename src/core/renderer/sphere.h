#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SPHERE
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SPHERE

#include "../math/sphere.h"
#include "material.h"
#include "../math/intersections.h"
#include "../math/ray.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	/// <summary>
	/// Represents an object within the rendered world.
	/// The Object is composed of a shape and a material
	/// </summary>
	class Sphere : public ::CppRayTracerChallenge::Core::Math::Sphere
	{
	public:
		Sphere();

		Sphere(Material material);

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
		Material m_material;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SPHERE
