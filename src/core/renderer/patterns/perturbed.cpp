#include "perturbed.h"

using CppRayTracerChallenge::Core::Renderer::Patterns::Perturbed;
using namespace CppRayTracerChallenge::Core;

Perturbed::Perturbed(const std::shared_ptr<Pattern> pattern) : m_pattern(pattern), m_perlin(Math::PerlinNoise()) {};

Graphics::Color Perturbed::colorAt(RML::Point position) const
{
	RML::Point patternLocal = m_pattern->transform().get_inverted() * position;

	double xIn = patternLocal.x();
	double yIn = patternLocal.y();
	double zIn = patternLocal.z();

	double x = xIn / (m_sampleWidth * m_scale) + m_offsetX;
	double y = yIn / (m_sampleHeight * m_scale) + m_offsetY;
	double z = zIn / (m_sampleDepth * m_scale) + m_offsetZ;

	float xNoise = static_cast<float>(m_perlin.at({ x, y, z }));
	float yNoise = static_cast<float>(m_perlin.at({ z, x, y }));
	float zNoise = static_cast<float>(m_perlin.at({ y, z, x }));

	float xOut = static_cast<float>(xIn) + (xNoise * m_intensity);
	float yOut = static_cast<float>(yIn) + (yNoise * m_intensity);
	float zOut = static_cast<float>(zIn) + (zNoise * m_intensity);

	return m_pattern->colorAt({ xOut, yOut, zOut });
};

bool Perturbed::operator==(const Perturbed& other) const
{
	return *m_pattern == *other.m_pattern;
}
