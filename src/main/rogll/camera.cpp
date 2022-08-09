#include "camera.h"

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

	const RML::Matrix<double, 4, 4> Camera::GetViewMatrix() const
	{
		return transform.matrix().invert();
	}

	const RML::Matrix<double, 4, 4> Camera::GetProjectionMatrix() const
	{
		return m_projectionMatrix;
	}
}
