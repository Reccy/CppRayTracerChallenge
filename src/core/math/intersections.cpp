#include <limits>
#include <optional>
#include "intersections.h"

using namespace CppRayTracerChallenge::Core::Math;

Intersections::Intersections() : m_intersections(std::vector<Intersection> {}) {};

Intersections::Intersections(const std::vector<Intersection> intersections) : m_intersections(intersections) {};

const Intersection& Intersections::at(unsigned int index) const
{
	return m_intersections.at(static_cast<size_t>(index));
}

const int Intersections::size() const
{
	return static_cast<int>(m_intersections.size());
}

const int Intersections::count() const
{
	return size();
}

const std::optional<const Intersection> Intersections::hit() const
{
	int index = -1;
	double tSmallest = std::numeric_limits<double>::max();

	for (unsigned int i = 0; i < m_intersections.size(); ++i)
	{
		const Intersection& intersection = m_intersections.at(i);

		if (intersection.t() < 0) continue;

		if (intersection.t() >= tSmallest) continue;

		tSmallest = intersection.t();
		index = i;
	}

	if (index < 0)
	{
		return std::nullopt;
	}

	return m_intersections.at(index);
}
