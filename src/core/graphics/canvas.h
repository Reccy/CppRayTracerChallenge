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
		Canvas(int width, int height) : m_width(width), m_height(height), m_size(width * height)
		{
			m_colors.reserve(m_size);
			
			for (int i = 0; i < m_size; i++)
			{
				m_colors.push_back(Color(0, 0, 0));
			}
		}

		~Canvas()
		{
			m_colors.clear();
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
		/// Returns a buffer with all of the Colors in left-right, top-down order
		/// </summary>
		std::vector<Color> toBuffer() const;
	private:
		int m_width, m_height, m_size;
		std::vector<Color> m_colors;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_CANVAS
