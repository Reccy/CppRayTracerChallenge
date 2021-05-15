#include <limits>
#include <optional>
#include <algorithm>
#include "intersections.h"
#include "intersection.h"

using namespace CppRayTracerChallenge::Core::Math;

Intersections::Intersections() : m_intersections(std::vector<Intersection> {}), m_hitIndex(-1) {};

Intersections::Intersections(const std::vector<Intersection> intersections) : m_hitIndex(-1)
{
	// Copy the intersection references into a vector of pointers so we can sort them
	std::vector<std::shared_ptr<Intersection>> intersectionsCopy;
	for (auto i = 0; i < intersections.size(); ++i)
	{
		intersectionsCopy.push_back(std::make_shared<Intersection>(intersections[i]));
	}

	std::sort(std::begin(intersectionsCopy), std::end(intersectionsCopy),
		[](const std::shared_ptr<Intersection> a, const std::shared_ptr<Intersection> b) -> bool
		{
			return a->t() < b->t();
		}
	);

	// Copy the sorted intersections back into m_intersections
	for (auto i = 0; i < intersectionsCopy.size(); ++i)
	{
		m_intersections.push_back(*intersectionsCopy[i]);
	}

	calculateHitindex();
};

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
	if (m_hitIndex < 0)
	{
		return std::nullopt;
	}

	return m_intersections.at(m_hitIndex);
}

Intersections Intersections::operator+(const Intersections& other) const
{
	Intersections result = Intersections();

	int thisIndex = 0;
	int otherIndex = 0;

	std::vector<Intersection>& resultCollection = result.m_intersections;

	auto thisIntersection = [this, &thisIndex]() -> const Intersection& {
		return this->m_intersections.at(thisIndex);
	};

	auto otherIntersection = [&other, &otherIndex]() -> const Intersection& {
		return other.m_intersections.at(otherIndex);
	};

	while (thisIndex < this->size() && otherIndex < other.size())
	{
		if (thisIntersection().t() < otherIntersection().t())
		{
			resultCollection.push_back(thisIntersection());
			++thisIndex;
		}
		else
		{
			resultCollection.push_back(otherIntersection());
			++otherIndex;
		}
	}

	while (thisIndex < this->size())
	{
		resultCollection.push_back(thisIntersection());
		++thisIndex;
	}

	while (otherIndex < other.size())
	{
		resultCollection.push_back(otherIntersection());
		++otherIndex;
	}

	result.calculateHitindex();

	return result;
}

Intersections Intersections::operator+=(const Intersections& other)
{
	*this = *this + other;
	return *this;
}

void Intersections::calculateHitindex()
{
	double tSmallest = std::numeric_limits<double>::max();

	for (unsigned int i = 0; i < m_intersections.size(); ++i)
	{
		const Intersection& intersection = m_intersections.at(i);

		if (intersection.t() < 0) continue;

		if (intersection.t() >= tSmallest) continue;

		tSmallest = intersection.t();
		m_hitIndex = i;
	}
}
