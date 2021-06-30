#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_PERTURBED
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_PERTURBED

#include "../pattern.h"
#include "../../math/perlin_noise.h"
#include "../../helpers/polymorphic_comparable.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	class Perturbed : public Helpers::PolymorphicComparable<Pattern, Perturbed>
	{
	public:
		/// <summary>
		/// Wraps an existing pattern and offsets the co-ordinates using Perlin Noise
		/// </summary>
		Perturbed(const std::shared_ptr<Pattern> pattern);

		Graphics::Color colorAt(Math::Point position) const override;

		bool operator==(const Perturbed& other) const;
	private:
		const std::shared_ptr<Pattern> m_pattern;
		const Math::PerlinNoise m_perlin;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_PATTERNS_PERTURBED
