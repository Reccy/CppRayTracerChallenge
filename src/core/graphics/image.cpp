#include "image.h"

using namespace CppRayTracerChallenge::Core::Graphics;

int Image::width() const
{
	return this->m_width;
}

int Image::height() const
{
	return this->m_height;
}

Color Image::readPixel(int x, int y) const
{
	return m_colors[x][y];
}

std::vector<std::vector<Color>> Image::toBuffer() const
{
	std::vector<std::vector<Color>> newVector = this->m_colors;
	return newVector;
}

