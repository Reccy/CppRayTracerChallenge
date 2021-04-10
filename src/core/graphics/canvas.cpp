#include "canvas.h"

using namespace CppRayTracerChallenge::Core::Graphics;

void Canvas::writePixel(int x, int y, const Color color)
{
	m_colors[this->pixelIndexAt(x,y)] = color;
}
