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
		explicit Image(int width, int height);

		/// <summary>
		/// Constructs an image of set width, height.
		/// Initializes color data to that of the passed in colors
		/// </summary>
		/// <param name="width">The width of the image</param>
		/// <param name="height">The height of the image</param>
		/// <param name="colors">The color data of the image</param>
		explicit Image(int width, int height, std::vector<Color> colors);

		/// <summary>
		/// Constructs an image os set width, height.
		/// Initializes all color data to the passed in color
		/// </summary>
		/// <param name="width">The width of the image</param>
		/// <param name="height">The height of the image</param>
		/// <param name="color">The color to set the image to</param>
		explicit Image(int width, int height, Color color);

		~Image();

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
		int pixelIndexAt(int x, int y) const;
	private:
		void initImage(Color color);
		void destroyImage();
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_IMAGE
