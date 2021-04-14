#include "color.h"

using namespace CppRayTracerChallenge::Core::Graphics;

Color::Color(const float red, const float green, const float blue) : m_tuple(Math::Tuple(red, green, blue, 1.0f)) {};

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
