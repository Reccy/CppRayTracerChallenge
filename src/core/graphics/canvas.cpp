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

void Canvas::writePixel(int x, int y, const Color color)
{
	m_colors[x][y] = color;
}

Color Canvas::readPixel(int x, int y) const
{
	return m_colors[x][y];
}

std::vector<std::vector<Color>> Canvas::toBuffer() const
{
	std::vector<std::vector<Color>> newVector = this->m_colors;
	return newVector;
}
