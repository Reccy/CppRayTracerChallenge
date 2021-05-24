#include "camera.h"
#include "../math/trig.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

Camera::Camera(int hSize, int vSize, int fieldOfView) :
	m_hSize(hSize), m_vSize(vSize), m_fieldOfView(fieldOfView), m_transformMatrix(Matrix<double>::identity(4))
{
	calculatePixelSize();
};

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

double Camera::pixelSize() const
{
	return m_pixelSize;
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

void Camera::calculatePixelSize()
{
	double halfView = tan(Math::Trig::degrees_to_radians(m_fieldOfView) / 2);
	double aspect = static_cast<double>(m_hSize) / static_cast<double>(m_vSize);

	double halfWidth;
	double halfHeight;

	if (aspect >= 1)
	{
		halfWidth = halfView;
		halfHeight = halfView / aspect;
	}
	else
	{
		halfWidth = halfView * aspect;
		halfHeight = halfView;
	}

	m_pixelSize = (halfWidth * 2) / m_hSize;
}
