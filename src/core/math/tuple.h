#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE

namespace CppRayTracerChallenge::Core::Math {
	/// <summary>
	/// A Tuple with x, y, z and w components
	/// </summary>
	class Tuple {
	public:
		/// <summary>
		/// Creates a tuple with x, y, z and w components
		/// </summary>
		/// <param name="x">X component</param>
		/// <param name="y">Y component</param>
		/// <param name="z">Z component</param>
		/// <param name="w">W component. 1 is a point, 2 is a tuple</param>
		Tuple(const float x, const float y, const float z, const float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

		/// <summary>
		/// Returns a readonly X component
		/// </summary>
		float getX();

		/// <summary>
		/// Returns a readonly Y component
		/// </summary>
		float getY();

		/// <summary>
		/// Returns a readonly Z component
		/// </summary>
		float getZ();

		/// <summary>
		/// Returns a readonly W component
		/// </summary>
		float getW();

		/// <summary>
		/// Returns if a tuple is a point
		/// </summary>
		/// <returns>True if point, False otherwise</returns>
		bool isPoint();

		/// <summary>
		/// Returns if a tuple is a vector
		/// </summary>
		/// <returns>True if vector, False otherwise</returns>
		bool isVector();
	private:
		float m_x, m_y, m_z, m_w;
	};
}

#endif // _CPPRAYTRACERCHALLENGE_CORE_MATH_TUPLE
