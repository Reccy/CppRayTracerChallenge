#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_LIGHTING
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_LIGHTING

#include "../graphics/color.h"
#include "../math/vector.h"
#include "../math/point.h"
#include "material.h"
#include "point_light.h"

namespace CppRayTracerChallenge::Core::Renderer::Lighting
{
	/// Computes the color at a given point with lighting
	Graphics::Color lighting(const Material& material, const PointLight& light, const Math::Point& position, const Math::Vector& eyev, const Math::Vector& normalv);
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_LIGHTING
