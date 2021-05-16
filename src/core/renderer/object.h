#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_OBJECT
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_OBJECT

#include "../math/sphere.h"
#include "material.h"
#include "../math/intersections.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class Object
	{
	public:
		Object(Math::Sphere shape);
		Object(Math::Sphere shape, Material material);

		const Material material() const;
		void material(const Material material);
		Math::Intersections intersect(const Math::Ray& ray) const;
	private:
		Math::Sphere m_shape;
		Material m_material;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_OBJECT
