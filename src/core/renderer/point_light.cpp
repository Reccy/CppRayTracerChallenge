#include "point_light.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

PointLight::PointLight(const RML::Point position, const Graphics::Color intensity) : m_position(position), m_intensity(intensity) {};

const RML::Point PointLight::position() const
{
	return m_position;
}

const Graphics::Color PointLight::intensity() const
{
	return m_intensity;
}
