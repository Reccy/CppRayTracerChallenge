#ifndef _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_IMAGE
#define _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_IMAGE

#include "color.h"
#include <vector>

namespace CppRayTracerChallenge::Core::Graphics
{
	/// <summary>
	/// A 2D image
	/// </summary>
	class Image
	{
	public:
		Image() = delete;

		/// <summary>
		/// Constructs an image of set width, height.
		/// Initializes all color data to black.
		/// </summary>
		/// <param name="width">The width of the image</param>
		/// <param name="height">The height of the image</param>
		explicit Image(int width, int height) : m_width(width), m_height(height), m_size { width * height }
		{
			initImage(Color::black());
		}

		/// <summary>
		/// Constructs an image of set width, height.
		/// Initializes color data to that of the passed in colors
		/// </summary>
		/// <param name="width">The width of the image</param>
		/// <param name="height">The height of the image</param>
		/// <param name="colors">The color data of the image</param>
		explicit Image(int width, int height, std::vector<Color> colors) : m_width(width), m_height(height), m_size{ width * height }, m_colors{ colors } {}

		/// <summary>
		/// Constructs an image os set width, height.
		/// Initializes all color data to the passed in color
		/// </summary>
		/// <param name="width">The width of the image</param>
		/// <param name="height">The height of the image</param>
		/// <param name="color">The color to set the image to</param>
		explicit Image(int width, int height, Color color) : m_width(width), m_height(height), m_size{ width * height }
		{
			initImage(color);
		}

		~Image()
		{
			destroyImage();
		}

		/// <summary>
		/// Returns the width of the image
		/// </summary>
		int width() const;

		/// <summary>
		/// Returns the height of the image
		/// </summary>
		int height() const;

		/// <summary>
		/// Returns a copy of the Color at the coordinates
		/// (0,0) is the top-left pixel
		/// </summary>
		/// <param name="x">The x coordinate to read from</param>
		/// <param name="y">The y coordinate to read from</param>
		/// <returns></returns>
		Color readPixel(int x, int y) const;

		/// <summary>
		/// Returns a buffer with all of the Colors in left-right, top-down order
		/// </summary>
		std::vector<Color> toBuffer() const;
	protected:
		int m_width, m_height, m_size;
		std::vector<Color> m_colors;

		/// <summary>
		/// Returns the index of the pixel at the x and y coordinates
		/// </summary>
		/// <returns>The index of the Color in m_colors</returns>
		int pixelIndexAt(int x, int y) const
		{
			return x + y * m_width;
		}
	private:
		void initImage(Color color)
		{
			m_colors.reserve(m_size);

			for (int i = 0; i < m_size; i++)
			{
				m_colors.push_back(color);
			}
		}

		void destroyImage()
		{
			m_colors.clear();
		}
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_IMAGE
