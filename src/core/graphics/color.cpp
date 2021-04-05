#include "color.h"

using namespace CppRayTracerChallenge::Core::Graphics;

float Color::red() const
{
	return this->m_tuple.x();
}

float Color::green() const
{
	return this->m_tuple.y();
}

float Color::blue() const
{
	return this->m_tuple.z();
}

bool Color::operator==(const Color& other) const
{
	return this->m_tuple == other.m_tuple;
}

bool Color::operator!=(const Color& other) const
{
	return !(*this == other);
}

Color Color::operator+(const Color& other) const
{
	return Color(
		this->red() + other.red(),
		this->green() + other.green(),
		this->blue() + other.blue()
	);
}
