#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CAMERA
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CAMERA

#include "../math/point.h"
#include "../math/vector.h"
#include "../math/transform.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	using namespace CppRayTracerChallenge::Core::Math;

	/// <summary>
	/// Represents a camera within the world
	/// </summary>
	class Camera
	{
	public:
		/// <summary>
		/// Creates a matrix that can be used to transform the world around the camera, effectively changing camera position
		/// </summary>
		/// <param name="from">The position of the camera in world space</param>
		/// <param name="to">The point that the camera is looking at</param>
		/// <param name="up">The up vector from the camera's point of view</param>
		/// <returns>The Matrix used to transform the world around the camera</returns>
		static Matrix<double> viewMatrix(const Point from, const Point to, const Vector up);
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CAMERA
