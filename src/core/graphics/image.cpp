#include "image.h"

namespace CppRayTracerChallenge::Core::Graphics
{
	Image::Image(int width, int height) : m_width(width), m_height(height), m_size(width* height)
	{
		initImage(Color::black());
	}

	Image::Image(int width, int height, std::vector<Color> colors) : m_width(width), m_height(height), m_size(width* height), m_colors(colors) {}

	Image::Image(int width, int height, Color color) : m_width(width), m_height(height), m_size(width* height)
	{
		initImage(color);
	}

	Image::~Image()
	{
		destroyImage();
	}

	int Image::width() const
	{
		return m_width;
	}

	int Image::height() const
	{
		return m_height;
	}

	Color Image::readPixel(int x, int y) const
	{
		return m_colors[this->pixelIndexAt(x, y)];
	}

	std::vector<Color> Image::toBuffer() const
	{
		std::vector<Color> newVector = this->m_colors;
		return newVector;
	}

	void Image::fromBuffer(std::vector<Color> buffer)
	{
		this->m_colors = buffer;
	}

	int Image::pixelIndexAt(int x, int y) const
	{
		return x + y * m_width;
	}

	void Image::initImage(Color color)
	{
		m_colors.reserve(m_size);

		for (int i = 0; i < m_size; i++)
		{
			m_colors.push_back(color);
		}
	}

	void Image::destroyImage()
	{
		m_colors.clear();
	}
}
