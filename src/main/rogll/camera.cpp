#include "camera.h"
#include <assert.h>
#include <string>
#include <sstream>

static double _CalculatePixelSize(double& outHalfWidth, double& outHalfHeight, double hSize, double vSize, double fovDegrees)
{
	double halfView = tan(RML::Trig::degrees_to_radians(fovDegrees) / 2);
	double aspect = static_cast<double>(hSize) / static_cast<double>(vSize);

	if (aspect >= 1)
	{
		outHalfWidth = halfView;
		outHalfHeight = halfView / aspect;
	}
	else
	{
		outHalfWidth = halfView * aspect;
		outHalfHeight = halfView;
	}

	return (outHalfWidth * 2) / hSize;
}

namespace ROGLL
{
	Camera::Camera(float width, float height)
		: transform()
	{
		SetOrthographic(width, height);
	}

	Camera::Camera(float width, float height, float fov)
		: transform()
	{
		SetPerspective(width, height, fov);
	}

	void Camera::SetOrthographic(float width, float height)
	{
		float w = 1.0f / width;
		float h = 1.0f / height;
		float x = -2.0f / (m_zFar - m_zNear);
		float y = -((m_zFar + m_zNear) / (m_zFar - m_zNear));

		m_projectionMatrix = RML::Matrix<double, 4, 4>({
			w, 0, 0, 0,
			0, h, 0, 0,
			0, 0, x, y,
			0, 0, 0, 1
		});
	}

	void Camera::SetPerspective(float width, float height, float fovRadians)
	{
		float a = (height / width) * (1 / tan(fovRadians/2));
		float b = (1/tan(fovRadians/2));
		float c = m_zFar / (m_zFar - m_zNear);
		float d = -((m_zFar * m_zNear) / (m_zFar - m_zNear));

		m_projectionMatrix = RML::Matrix<double, 4, 4>({
			a, 0, 0, 0,
			0, b, 0, 0,
			0, 0, c, d,
			0, 0, 1, 0
		});
	}

	const CppRayTracerChallenge::Core::Math::Ray Camera::RayForPixel(const double x, const double y, const double renderWidth, const double renderHeight, const double fov) const
	{
		double xPixel = renderWidth - x; // OpenGL space positive X is opposite to world space positive X
		double yPixel = y;

		double halfWidth;
		double halfHeight;
		double pixelSize = _CalculatePixelSize(halfWidth, halfHeight, renderWidth, renderHeight, fov);

		double xOffset = (xPixel) * pixelSize;
		double yOffset = (yPixel) * pixelSize;

		double xWorld = halfWidth - xOffset;
		double yWorld = halfHeight - yOffset;

		RML::Point worldPosition = transform.matrix() * RML::Point(xWorld, yWorld, 0.75);
		RML::Point origin = transform.matrix() * RML::Point(0, 0, 0);

		RML::Vector direction = RML::Vector(worldPosition - origin).normalized();

		return CppRayTracerChallenge::Core::Math::Ray(origin, direction);
	}

	const RML::Matrix<double, 4, 4> Camera::GetViewMatrix() const
	{
		return transform.matrix().invert();
	}

	const RML::Matrix<double, 4, 4> Camera::GetProjectionMatrix() const
	{
		return m_projectionMatrix;
	}
}
