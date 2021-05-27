#include "color.h"
#include "../math/comparison.h"

using namespace CppRayTracerChallenge::Core::Graphics;
using namespace CppRayTracerChallenge::Core::Math;

Color::Color(const float red, const float green, const float blue) : m_red(red), m_green(green), m_blue(blue) {};

Color Color::black()
{
	return Color(0, 0, 0);
}

Color Color::white()
{
	return Color(1, 1, 1);
}

float Color::red() const
{
	return m_red;
}

float Color::green() const
{
	return m_green;
}

float Color::blue() const
{
	return m_blue;
}

bool Color::operator==(const Color& other) const
{
	return Comparison::equal(m_red, other.m_red) &&
		Comparison::equal(m_green, other.m_green) &&
		Comparison::equal(m_blue, other.m_blue);
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

Color Color::operator-(const Color& other) const
{
	return Color(
		this->red() - other.red(),
		this->green() - other.green(),
		this->blue() - other.blue()
	);
}

Color Color::operator*(const float scalar) const
{
	return Color(
		this->red() * scalar,
		this->green() * scalar,
		this->blue() * scalar
	);
}

Color Color::operator*(const Color& other) const
{
	return Color(
		this->red() * other.red(),
		this->green() * other.green(),
		this->blue() * other.blue()
	);
}

std::ostream& CppRayTracerChallenge::Core::Graphics::operator<<(std::ostream& os, const Color& color) {
	os << "R(" << color.red() << "), G(" << color.green() << "), B(" << color.blue() << ")";
	return os;
}
