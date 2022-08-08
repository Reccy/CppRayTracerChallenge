#pragma once
#include "../pattern.h"
#include "../../helpers/polymorphic_comparable.h"
#include "../../math/transform.h"

namespace CppRayTracerChallenge::Core::Renderer::Patterns
{
	template<typename PATTERN_MASK>
	class Masked : public Helpers::PolymorphicComparable<Pattern, Masked<PATTERN_MASK>>
	{
	public:
		Masked(const std::shared_ptr<Pattern> a, const std::shared_ptr<Pattern> b) :
			m_a(a),
			m_b(b),
			m_mask(PATTERN_MASK(Graphics::Color::black(), Graphics::Color::white()))
		{};

		Masked(const std::shared_ptr<Pattern> a, const std::shared_ptr<Pattern> b, const Math::Transform maskTransform) :
			m_a(a),
			m_b(b),
			m_mask(PATTERN_MASK(Graphics::Color::black(), Graphics::Color::white()))
		{
			m_mask.transform(maskTransform);
		};

		Graphics::Color colorAt(RML::Point position) const override
		{
			// Since we have a black to white image, the rgb values are all equal.
			float blend = m_mask.colorAt(position).red();

			RML::Point positionA = m_a->transform().invert() * position;
			RML::Point positionB = m_b->transform().invert() * position;

			Graphics::Color a = m_a->colorAt(positionA);
			Graphics::Color b = m_b->colorAt(positionB);

			return Graphics::Color::blend(a, b, blend);
		};

		bool operator==(const Masked& other) const
		{
			return m_a == other.m_a && m_b == other.m_b && m_mask == other.m_mask;
		};
	private:
		const std::shared_ptr<Pattern> m_a;
		const std::shared_ptr<Pattern> m_b;
		PATTERN_MASK m_mask;
	};
}
