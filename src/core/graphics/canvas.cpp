#include "canvas.h"

using namespace CppRayTracerChallenge::Core::Graphics;

int Canvas::width() const
{
	return this->m_width;
}

int Canvas::height() const
{
	return this->m_height;
}

std::vector<Color> Canvas::toBuffer() const
{
	std::vector newVector = m_colors;
	return newVector;
}
