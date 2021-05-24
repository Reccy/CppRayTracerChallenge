#include "camera.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;

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
