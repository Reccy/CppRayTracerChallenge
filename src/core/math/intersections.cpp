#include "intersections.h"
#include "intersection.cpp"

Intersections::Intersections(const std::vector<std::reference_wrapper<const Intersection>> intersections) : m_intersections(intersections) {};

const int Intersections::count() const
{
	return static_cast<int>(m_intersections.size());
}
