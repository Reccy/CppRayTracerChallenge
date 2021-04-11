#include "canvas.h"

using namespace CppRayTracerChallenge::Core::Graphics;

void Canvas::writePixel(int x, int y, const Color color)
{
	if (x < 0 || x >= this->m_width) {
		return;
	}

	if (y < 0 || y >= this->m_height) {
		return;
	}

	m_colors[this->pixelIndexAt(x,y)] = color;
}
