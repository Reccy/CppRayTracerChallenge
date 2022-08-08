#include "lighting.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	Graphics::Color Lighting::lighting(const Shape& shape, const PointLight& light, const RML::Point& worldPosition, const RML::Vector& eyev, const RML::Vector& normalv, const bool isShadowed)
	{
		const Renderer::Material& material = shape.material();
		Graphics::Color effectiveColor = shape.colorAt(worldPosition) * light.intensity();
		RML::Vector lightv = (light.position() - worldPosition).normalize();
		Graphics::Color ambient = effectiveColor * material.ambient;

		if (isShadowed)
		{
			return ambient;
		}

		float lightDotNormal = static_cast<float>(RML::Vector::dot(lightv, normalv));

		Graphics::Color diffuse = Graphics::Color::black();
		Graphics::Color specular = Graphics::Color::black();

		if (lightDotNormal >= 0)
		{
			diffuse = effectiveColor * material.diffuse * lightDotNormal;

			RML::Vector reflectv = RML::Vector::reflect(-lightv, normalv);
			double reflectDotEye = RML::Vector::dot(reflectv, eyev);

			if (reflectDotEye > 0)
			{
				float factor = static_cast<float>(pow(reflectDotEye, material.shininess));
				specular = light.intensity() * material.specular * factor;
			}
		}

		return ambient + diffuse + specular;
	}
}
