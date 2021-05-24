#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CAMERA
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CAMERA

#include "../math/point.h"
#include "../math/vector.h"
#include "../math/transform.h"
#include "../math/ray.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	using namespace CppRayTracerChallenge::Core::Math;

	/// <summary>
	/// Represents a camera within the world
	/// </summary>
	class Camera
	{
	public:
		Camera() = delete;

		/// <summary>
		/// Creates a Camera with a horizontal and vertical pixel image size.
		/// Also sets the camera FOV.
		/// </summary>
		/// <param name="hSize">Horizontal size in pixels</param>
		/// <param name="vSize">Vertical size in pixels</param>
		/// <param name="fieldOfView">Field of view in degrees</param>
		Camera(int hSize, int vSize, int fieldOfView);

		/// <summary>
		/// The horizontal size of the camera in pixels
		/// </summary>
		/// <returns>Horizontal size</returns>
		int hSize() const;

		/// <summary>
		/// The vertical size of the camera in pixels
		/// </summary>
		/// <returns>Vertical size</returns>
		int vSize() const;

		/// <summary>
		/// The field of view in degrees
		/// </summary>
		/// <returns>FOV</returns>
		int fieldOfView() const;

		/// <summary>
		/// The size of a pixel in respect to the camera
		/// </summary>
		/// <returns>The pixel size</returns>
		double pixelSize() const;

		/// <summary>
		/// Transforms the camera by the passed in transform
		/// </summary>
		/// <param name="transform">The transform to apply to the camera</param>
		void transform(const Math::Transform& transform);

		/// <summary>
		/// Calculates the Ray that will go from the camera origin through the pixel on the canvas
		/// </summary>
		/// <param name="xPixel"><The horizontal pixel to cast a ray through/param>
		/// <param name="yPixel">The vertical pixel to cast a ray through</param>
		/// <returns></returns>
		Math::Ray Camera::rayForPixel(int xPixel, int yPixel) const;

		/// <summary>
		/// The underlying transform matrix for the camera
		/// </summary>
		/// <returns>The transform matrix</returns>
		Matrix<double> transformMatrix() const;

		/// <summary>
		/// Creates a matrix that can be used to transform the world around the camera, effectively changing camera position
		/// </summary>
		/// <param name="from">The position of the camera in world space</param>
		/// <param name="to">The point that the camera is looking at</param>
		/// <param name="up">The up vector from the camera's point of view</param>
		/// <returns>The Matrix used to transform the world around the camera</returns>
		static Matrix<double> viewMatrix(const Point from, const Point to, const Vector up);
	private:
		int m_hSize;
		int m_vSize;
		int m_fieldOfView;
		double m_halfWidth;
		double m_halfHeight;
		double m_pixelSize;
		Matrix<double> m_transformMatrix;

		void calculatePixelSize();
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_CAMERA
