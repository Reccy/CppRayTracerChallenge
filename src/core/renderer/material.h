#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_MATERIAL
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_MATERIAL

#include "../math/comparison.h"
#include "../graphics/color.h"
#include "pattern.h"
#include "patterns/solid_color.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	enum class ShadowcastMode
	{
		ALWAYS,
		NEVER,
		WHEN_TRANSPARENT
	};

	/// <summary>
	/// Represents a Material struct for use in a Phong Lighting Model
	/// </summary>
	struct Material
	{
		std::shared_ptr<Pattern> pattern = std::make_shared<Patterns::SolidColor>(Patterns::SolidColor(Graphics::Color::white()));
		float ambient = 0.1f;
		float diffuse = 0.9f;
		float specular = 0.9f;
		float shininess = 200.0f;
		float reflective = 0.0f;
		float transparency = 0.0f;
		float refractiveIndex = 1.0f;
		ShadowcastMode shadowcastMode = ShadowcastMode::WHEN_TRANSPARENT;
	};

	bool operator==(const Material& lhs, const Material& rhs);
	bool operator!=(const Material& lhs, const Material& rhs);
	std::ostream& operator<<(std::ostream& os, const Material& material);
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_MATERIAL
