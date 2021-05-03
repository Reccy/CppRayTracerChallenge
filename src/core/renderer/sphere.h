#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SPHERE
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SPHERE

#include "../math/sphere.h"
#include "material.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	/// <summary>
	/// A 3D Sphere that contains extra information for rendering
	/// </summary>
	class Sphere : public ::CppRayTracerChallenge::Core::Math::Sphere
	{
	public:
		/// <summary>
		/// Constructs a unit sphere at position 0,0,0 and sets the default material
		/// </summary>
		Sphere();

		/// <summary>
		/// Returns the material belonging to the sphere
		/// </summary>
		/// <returns>The material belonging to the sphere</returns>
		const Material material() const;

		/// <summary>
		/// Sets the material on the sphere
		/// </summary>
		/// <param name="material">The material to set on the sphere</param>
		/// <returns>New Sphere after having the material set</returns>
		void material(const Material material);
	private:
		Material m_material;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_SPHERE
