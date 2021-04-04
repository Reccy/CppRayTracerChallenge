#ifndef _CPPRAYTRACERCHALLENGE_CORE_MATH_I_CARTESIAN
#define _CPPRAYTRACERCHALLENGE_CORE_MATH_I_CARTESIAN

namespace CppRayTracerChallenge::Core::Math {

	/// <summary>
	/// Represents a structure that can return x, y, z and w cartesian components
	/// </summary>
	/// <typeparam name="T">The datatype of the coordinates</typeparam>
	template<typename T>
	class ICartesian {
	public:
		/// <summary>
		/// Returns a const X component
		/// </summary>
		virtual T x() const = 0;

		/// <summary>
		/// Returns a const Y component
		/// </summary>
		virtual T y() const = 0;

		/// <summary>
		/// Returns a const Z component
		/// </summary>
		virtual T z() const = 0;

		/// <summary>
		/// Returns a const W component
		/// </summary>
		virtual T w() const = 0;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_MATH_I_CARTESIAN
