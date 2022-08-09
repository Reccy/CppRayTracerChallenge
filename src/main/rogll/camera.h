#pragma once
#include <RML.h>

namespace ROGLL
{
	class Camera
	{
	public:
		RML::Transform transform;
	public:
		/// <summary>
		/// Creates a Camera with orthographic projection
		/// </summary>
		Camera(float width, float height);

		/// <summary>
		/// Creates a Camera with perspective projection
		/// </summary>
		Camera(float width, float height, float fov);

		void SetOrthographic(float width, float height);
		void SetPerspective(float width, float height, float fovRadians);

		const RML::Matrix<double, 4, 4> GetViewMatrix() const;
		const RML::Matrix<double, 4, 4> GetProjectionMatrix() const;
	private:
		RML::Matrix<double, 4, 4> m_projectionMatrix;
		
		float m_zNear = 0.1f;
		float m_zFar = 1000.0f;
	};
}
