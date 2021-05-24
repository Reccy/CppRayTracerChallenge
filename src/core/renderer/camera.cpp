#include "camera.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

Camera::Camera(int hSize, int vSize, int fieldOfView) :
	m_hSize(hSize), m_vSize(vSize), m_fieldOfView(fieldOfView), m_transformMatrix(Matrix<double>::identity(4)) {};

int Camera::hSize() const
{
	return m_hSize;
}

int Camera::vSize() const
{
	return m_vSize;
}

int Camera::fieldOfView() const
{
	return m_fieldOfView;
}

Matrix<double> Camera::transformMatrix() const
{
	return m_transformMatrix;
}

Matrix<double> Camera::viewMatrix(const Point from, const Point to, const Vector up)
{
	Vector forward = (to - from).normalize();
	Vector upNormalized = up.normalize();
	Vector left = Vector::cross(forward, upNormalized);
	Vector trueUp = Vector::cross(left, forward);

	Matrix<double> orientation = Matrix<double>(4, 4, std::vector<double> {
		left.x(), left.y(), left.z(), 0,
		trueUp.x(), trueUp.y(), trueUp.z(), 0,
		-forward.x(), -forward.y(), -forward.z(), 0,
		0, 0, 0, 1
	});

	return orientation * Transform().translate(-from.x(), -from.y(), -from.z()).matrix();
}
