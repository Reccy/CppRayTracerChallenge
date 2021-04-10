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
		explicit Image(int width, int height) : m_width(width), m_height(height)
		{
			initBlankImage();
		}

		/// <summary>
		/// Constructs an image of set width, height.
		/// Initializes color data to that of the passed in colors
		/// </summary>
		/// <param name="width">The width of the image</param>
		/// <param name="height">The height of the image</param>
		/// <param name="colors">The color data of the image</param>
		explicit Image(int width, int height, std::vector<Color> colors) : m_width(width), m_height(height)
		{
			initColorImage(colors);
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
		std::vector<std::vector<Color>> toBuffer() const;
	protected:
		int m_width, m_height;
		std::vector<std::vector<Color>> m_colors;
	private:
		void initBlankImage()
		{
			m_colors.reserve(m_width);

			for (int x = 0; x < m_width; x++)
			{
				std::vector<Color> colorColumn;
				colorColumn.reserve(m_height);

				for (int y = 0; y < m_height; y++)
				{
					colorColumn.push_back(Color(0, 0, 0));
				}

				m_colors.push_back(colorColumn);
			}
		}

		void initColorImage(std::vector<Color> colors)
		{
			m_colors.reserve(m_width);

			int i = 0;

			for (int x = 0; x < m_width; x++)
			{
				std::vector<Color> colorColumn;
				colorColumn.reserve(m_height);

				for (int y = 0; y < m_height; y++)
				{
					colorColumn.push_back(colors[i]);
					i++;
				}

				m_colors.push_back(colorColumn);
			}
		}

		void destroyImage()
		{
			for (int x = 0; x < m_width; x++)
			{
				m_colors.at(x).clear();
			}

			m_colors.clear();
		}
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_IMAGE
