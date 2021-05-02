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
