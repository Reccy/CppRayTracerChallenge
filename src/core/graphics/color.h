#ifndef _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_COLOR
#define _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_COLOR

#include "../math/tuple.h"

using namespace CppRayTracerChallenge::Core;

namespace CppRayTracerChallenge::Core::Graphics
{
	class Color {
	public:
		Color() = delete;

		/// <summary>
		/// Constructs a Color with red, green and blue channels
		/// </summary>
		/// <param name="red">Red channel</param>
		/// <param name="green">Green channel</param>
		/// <param name="blue">Blue channel</param>
		Color(const float red, const float green, const float blue) : m_tuple(Math::Tuple(red, green, blue, 1.0f)) {};

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


		bool operator==(const Color& other) const;
		bool operator!=(const Color& other) const;
		Color operator+(const Color& other) const;
		Color operator-(const Color& other) const;
		Color operator*(const float scalar) const;
		Color operator*(const Color& other) const;
	private:
		Math::Tuple<float> m_tuple;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_GRAPHICS_COLOR
