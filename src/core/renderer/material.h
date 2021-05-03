#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_MATERIAL
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_MATERIAL

#include "../math/comparison.h"
#include "../graphics/color.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	/// <summary>
	/// Represents a Material struct for use in a Phong Lighting Model
	/// </summary>
	struct Material
	{
		Graphics::Color color = Graphics::Color::white();
		double ambient = 0.1;
		double diffuse = 0.9;
		double specular = 0.9;
		double shininess = 200.0;
	};

	bool operator==(const Material& lhs, const Material& rhs);
	bool operator!=(const Material& lhs, const Material& rhs);
	std::ostream& operator<<(std::ostream& os, const Material& material);
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_MATERIAL
