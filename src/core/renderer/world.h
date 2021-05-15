#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_WORLD
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_WORLD

#include "point_light.h"
#include "sphere.h"
#include "../math/ray.h"
#include "../math/intersections.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	/// <summary>
	/// Represents a world composed of multiple objects and light sources that can be rendererd to a canvas
	/// </summary>
	class World
	{
	public:
		/// <summary>
		/// Constructs a World that is completely empty
		/// </summary>
		World();

		/// <summary>
		/// Constructs a World that has a single light source and two spheres
		/// </summary>
		static World defaultWorld();

		/// <summary>
		/// Adds a light to the world
		/// </summary>
		/// <param name="light">The light to add</param>
		/// <returns>The world, after the light being added to it</returns>
		World addLight(PointLight light);

		/// <summary>
		/// Adds a object to the world
		/// </summary>
		/// <param name="obj">The object to add</param>
		/// <returns>The world, after the object being added to it</returns>
		World addObject(Sphere obj);

		/// <summary>
		/// Returns how many objects are in the world
		/// </summary>
		/// <returns>How many objects are in the world</returns>
		int objectCount() const;

		/// <summary>
		/// Returns how many lights are in the world
		/// </summary>
		/// <returns>How many lights are in the world</returns>
		int lightCount() const;

		/// <summary>
		/// Returns the intersections between objects in the world and the ray
		/// </summary>
		/// <param name="ray">The ray to cast in the world</param>
		/// <returns>Intersections</returns>
		Math::Intersections intersect_ray(const Math::Ray ray) const;
	private:
		std::vector<PointLight> m_lights;
		std::vector<Sphere> m_objects;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_WORLD
