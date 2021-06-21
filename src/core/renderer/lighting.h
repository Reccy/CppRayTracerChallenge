#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_LIGHTING
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_LIGHTING

#include "../graphics/color.h"
#include "../math/vector.h"
#include "../math/point.h"
#include "material.h"
#include "point_light.h"
#include "../renderer/shape.h"

namespace CppRayTracerChallenge::Core::Renderer::Lighting
{
	/// <summary>
	/// Computes the color at a given point with lighting
	/// </summary>
	/// <param name="shape">The intersected object to render</param>
	/// <param name="light">The point light that is lighting the area</param>
	/// <param name="position">The position on the surface that is being lit</param>
	/// <param name="eyev">The eye vector to the camera</param>
	/// <param name="normalv">The normal vector of the surface</param>
	/// <param name="isShadowed">If the position is in front of or behind the light</param>
	/// <returns>The color at the position</returns>
	Graphics::Color lighting(const Shape& shape, const PointLight& light, const Math::Point& position, const Math::Vector& eyev, const Math::Vector& normalv, const bool isShadowed);
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_LIGHTING
