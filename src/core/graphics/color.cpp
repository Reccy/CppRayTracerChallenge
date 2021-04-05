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
