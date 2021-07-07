#include "material_helper.h"

CppRayTracerChallenge::Core::Renderer::Shape CppRayTracerChallenge::Core::Helpers::MaterialHelper::glassSphere()
{
	std::shared_ptr<Math::Sphere> sphere = std::make_shared<Math::Sphere>();

	Renderer::Material material;
	material.transparency = 1.0f;
	material.refractiveIndex = 1.5f;

	Renderer::Shape shape = Renderer::Shape(sphere, material);
	return shape;
}
