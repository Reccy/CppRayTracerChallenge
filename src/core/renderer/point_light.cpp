#include "point_light.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core;

PointLight::PointLight() : m_position(), m_intensity({ 1, 1, 1 }) {};

PointLight::PointLight(const Math::Point position, const Graphics::Color intensity) : m_position(position), m_intensity(intensity) {};

const Math::Point PointLight::position() const
{
	return m_position;
}

const Graphics::Color PointLight::intensity() const
{
	return m_intensity;
}
