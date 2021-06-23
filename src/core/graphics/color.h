#ifndef _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_COLOR
#define _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_COLOR

#include <iostream>

namespace CppRayTracerChallenge::Core::Graphics
{
	class Color {
	public:
		Color() = delete;

		/// <summary>
		/// Constructs a Color with red, green and blue channels.
		/// All channels have a range of 0.0f (minimum) to 1.0f (max) intensity.
		/// </summary>
		/// <param name="red">Red channel</param>
		/// <param name="green">Green channel</param>
		/// <param name="blue">Blue channel</param>
		Color(const float red, const float green, const float blue);

		/// <summary>
		/// Returns a black color
		/// </summary>
		static Color black();

		/// <summary>
		/// Returns a white color
		/// </summary>
		static Color white();

		/// <summary>
		/// Returns the value of the red channel
		/// </summary>
		float red() const;

		/// <summary>
		/// Returns the value of the green channel
		/// </summary>
		/// <returns></returns>
		float green() const;

		/// <summary>
		/// Returns the value of the blue channel
		/// </summary>
		float blue() const;

		/// <summary>
		/// Linearly interpolates between the two colors
		/// </summary>
		/// <param name="a">Color A</param>
		/// <param name="b">Color B</param>
		/// <param name="t">Float for combining a and b</param>
		/// <returns>Returns the blended color</returns>
		static Color blend(const Color& a, const Color& b, float t);

		bool operator==(const Color& other) const;
		bool operator!=(const Color& other) const;
		Color operator+(const Color& other) const;
		Color operator-(const Color& other) const;
		Color operator*(const float scalar) const;
		Color operator*(const Color& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Color& color);
	private:
		float m_red, m_green, m_blue;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_COLOR
