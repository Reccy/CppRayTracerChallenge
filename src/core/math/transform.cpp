#include "transform.h"
#include "trig.h"
#include <vector>

using namespace CppRayTracerChallenge::Core::Math;

Transform::Transform() : m_matrix(Matrix<double>::identity(4)) {};

const Transform Transform::translate(const double x, const double y, const double z) const
{
	Transform copy = *this;

	Matrix<double> translationMatrix = Matrix<double>::identity(4);
	translationMatrix(0, 3) = x;
	translationMatrix(1, 3) = y;
	translationMatrix(2, 3) = z;

	copy.m_matrices.push(translationMatrix);

	return copy;
}

const Transform Transform::scale(const double x, const double y, const double z) const
{
	Transform copy = *this;

	Matrix<double> scaleMatrix = Matrix<double>::identity(4);
	scaleMatrix(0, 0) = x;
	scaleMatrix(1, 1) = y;
	scaleMatrix(2, 2) = z;

	copy.m_matrices.push(scaleMatrix);

	return copy;
}

const Transform Transform::rotate(const double x, const double y, const double z) const
{
	Transform copy = *this;

	const double xRad = Trig::degrees_to_radians(x);
	const double yRad = Trig::degrees_to_radians(y);
	const double zRad = Trig::degrees_to_radians(z);

	Matrix<double> xRotationMatrix = Matrix<double>(4, 4, std::vector<double> {
		1, 0, 0, 0,
			0, cos(xRad), -sin(xRad), 0,
			0, sin(xRad), cos(xRad), 0,
			0, 0, 0, 1
	});

	Matrix<double> yRotationMatrix = Matrix<double>(4, 4, std::vector<double> {
		cos(yRad), 0, sin(yRad), 0,
			0, 1, 0, 0,
			-sin(yRad), 0, cos(yRad), 0,
			0, 0, 0, 1
	});

	Matrix<double> zRotationMatrix = Matrix<double>(4, 4, std::vector<double> {
		cos(zRad), -sin(zRad), 0, 0,
			sin(zRad), cos(zRad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
	});

	Matrix<double> rotationMatrix = Matrix<double>::identity(4);

	rotationMatrix = rotationMatrix * xRotationMatrix;
	rotationMatrix = rotationMatrix * yRotationMatrix;
	rotationMatrix = rotationMatrix * zRotationMatrix;

	copy.m_matrices.push(rotationMatrix);

	return copy;
}

const Transform Transform::shear(const double xY, const double xZ, const double yX, const double yZ, const double zX, const double zY) const
{
	Transform copy = *this;

	Matrix<double> shearMatrix = Matrix<double>(4, 4, std::vector<double> {
		1, xY, xZ, 0,
		yX, 1, yZ, 0,
		zX, zY, 1, 0,
		0, 0, 0, 1
	});

	copy.m_matrices.push(shearMatrix);

	return copy;
}

const Transform Transform::invert() const
{
	Transform copy = *this;

	copy.calculateMatrix();

	if (copy.m_matrix.invertible())
	{
		copy.m_matrix = copy.m_matrix.invert();
	}

	return copy;
}

Tuple<double> Transform::operator*(const Tuple<double>& tuple) const
{
	Transform copy = *this;

	copy.calculateMatrix();

	return copy.m_matrix * tuple;
}

void Transform::calculateMatrix()
{
	while (!m_matrices.empty())
	{
		m_matrix = m_matrix * m_matrices.top();
		m_matrices.pop();
	}
}

bool Transform::operator==(const Transform& other) const
{
	Transform copy = *this;
	Transform otherCopy = other;

	copy.calculateMatrix();
	otherCopy.calculateMatrix();

	return copy.m_matrix == otherCopy.m_matrix;
}

bool Transform::operator!=(const Transform& other) const
{
	return !((*this) == other);
}
