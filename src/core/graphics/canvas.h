#ifndef _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_CANVAS
#define _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_CANVAS

#include <vector>
#include "color.h"

namespace CppRayTracerChallenge::Core::Graphics
{
	/// <summary>
	/// A 2D canvas to write pixel color data onto
	/// </summary>
	class Canvas
	{
	public:
		Canvas() = delete;

		/// <summary>
		/// Constructs a Canvas of set width and height.
		/// Initializes all color data to black
		/// </summary>
		/// <param name="width">The width of the canvas</param>
		/// <param name="height">The height of the canvas</param>
		Canvas(int width, int height) : m_width(width), m_height(height)
		{
			constructColors();
		}

		~Canvas()
		{
			destructColors();
		}

		/// <summary>
		/// Returns the width of the canvas
		/// </summary>
		int width() const;

		/// <summary>
		/// Returns the height of the canvas
		/// </summary>
		int height() const;

		/// <summary>
		/// Writes color data to the specified coordinate on the canvas
		/// (0,0) is the top-left pixel
		/// </summary>
		/// <param name="x">The x coordinate to write to</param>
		/// <param name="y">The y coordinate to write to</param>
		/// <param name="color">The color to write to the coordinates</param>
		void writePixel(int x, int y, const Color color);

		/// <summary>
		/// Returns a copy of the Color at the coordinates
		/// (0,0) is the top-left pixel
		/// </summary>
		/// <param name="x">The x coordinate to read from</param>
		/// <param name="y">The y coordinate to read from</param>
		/// <returns></returns>
		Color readPixel(int x, int y);

		/// <summary>
		/// Returns a buffer with all of the Colors in left-right, top-down order
		/// </summary>
		std::vector<std::vector<Color>> toBuffer() const;
	private:
		int m_width, m_height;
		std::vector<std::vector<Color>> m_colors;

		void constructColors()
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

		void destructColors()
		{
			for (int x = 0; x < m_width; x++)
			{
				m_colors.at(x).clear();
			}

			m_colors.clear();
		}
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_CANVAS
