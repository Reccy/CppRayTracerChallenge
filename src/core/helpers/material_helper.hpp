#ifndef _CPPRAYTRACERCHALLENGE_CORE_HELPERS_MATERIAL_HELPER
#define _CPPRAYTRACERCHALLENGE_CORE_HELPERS_MATERIAL_HELPER

#include "../renderer/material.h"
#include "../renderer/shape.h"
#include "../math/sphere.h"

namespace CppRayTracerChallenge::Core::Helpers::MaterialHelper
{
	Renderer::Shape glassSphere()
	{
		std::shared_ptr<Math::Sphere> sphere = std::make_shared<Math::Sphere>();

		Renderer::Material material;
		material.transparency = 1.0f;
		material.refractiveIndex = 1.5f;

		Renderer::Shape shape = Renderer::Shape(sphere, material);
		return shape;
	}
}

#endif _CPPRAYTRACERCHALLENGE_CORE_HELPERS_MATERIAL_HELPER
