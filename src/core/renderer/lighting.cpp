#include "lighting.h"

using namespace CppRayTracerChallenge::Core;

Graphics::Color CppRayTracerChallenge::Core::Renderer::Lighting::lighting(const Material& material, const PointLight& light, const Math::Point& position, const Math::Vector& eyev, const Math::Vector& normalv, const bool isShadowed)
{
	Graphics::Color effectiveColor = material.color * light.intensity();
	Math::Vector lightv = (light.position() - position).normalize();
	Graphics::Color ambient = effectiveColor * material.ambient;

	if (isShadowed)
	{
		return ambient;
	}

	float lightDotNormal = static_cast<float>(Math::Vector::dot(lightv, normalv));

	Graphics::Color diffuse = Graphics::Color::black();
	Graphics::Color specular = Graphics::Color::black();

	if (lightDotNormal >= 0)
	{
		diffuse = effectiveColor * material.diffuse * lightDotNormal;

		Math::Vector reflectv = Math::Vector::reflect(-lightv, normalv);
		double reflectDotEye = Math::Vector::dot(reflectv, eyev);

		if (reflectDotEye > 0)
		{
			float factor = static_cast<float>(pow(reflectDotEye, material.shininess));
			specular = light.intensity() * material.specular * factor;
		}
	}

	return ambient + diffuse + specular;
}
