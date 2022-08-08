#pragma once
#include <vector>
#include "image.h"
#include "color.h"

namespace CppRayTracerChallenge::Core::Graphics
{
	/// <summary>
	/// A 2D canvas to write pixel color data onto
	/// </summary>
	class Canvas : public Image
	{
	public:
		Canvas() = delete;

		using Image::Image;

		/// <summary>
		/// Writes color data to the specified coordinate on the canvas
		/// (0,0) is the top-left pixel
		/// </summary>
		/// <param name="x">The x coordinate to write to</param>
		/// <param name="y">The y coordinate to write to</param>
		/// <param name="color">The color to write to the coordinates</param>
		void writePixel(int x, int y, const Color color);
	};
}
