#include "transform.h"
#include "trig.h"
#include <vector>

using namespace CppRayTracerChallenge::Core::Math;

Transform::Transform() : m_matrix(Matrix<double>::identity(4)) {};

Transform Transform::translate(const double x, const double y, const double z)
{
	Matrix<double> translationMatrix = Matrix<double>::identity(4);
	translationMatrix(0, 3) = x;
	translationMatrix(1, 3) = y;
	translationMatrix(2, 3) = z;

	m_matrices.push(translationMatrix);

	return *this;
}

Transform Transform::scale(const double x, const double y, const double z)
{
	Matrix<double> scaleMatrix = Matrix<double>::identity(4);
	scaleMatrix(0, 0) = x;
	scaleMatrix(1, 1) = y;
	scaleMatrix(2, 2) = z;

	m_matrices.push(scaleMatrix);

	return *this;
}

Transform Transform::rotate(const double x, const double y, const double z)
{
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

	m_matrices.push(rotationMatrix);

	return *this;
}

Transform Transform::shear(const double xY, const double xZ, const double yX, const double yZ, const double zX, const double zY)
{
	Matrix<double> shearMatrix = Matrix<double>(4, 4, std::vector<double> {
		1, xY, xZ, 0,
		yX, 1, yZ, 0,
		zX, zY, 1, 0,
		0, 0, 0, 1
	});

	m_matrices.push(shearMatrix);

	return *this;
}

Transform Transform::invert()
{
	calculateMatrix();

	if (m_matrix.invertible())
	{
		m_matrix = m_matrix.invert();
	}

	return *this;
}

Tuple<double> Transform::operator*(const Tuple<double>& tuple)
{
	calculateMatrix();

	return m_matrix * tuple;
}

void Transform::calculateMatrix()
{
	while (!m_matrices.empty())
	{
		m_matrix = m_matrix * m_matrices.top();
		m_matrices.pop();
	}
}

bool Transform::operator==(Transform& other)
{
	calculateMatrix();
	other.calculateMatrix();

	return m_matrix == other.m_matrix;
}

bool Transform::operator!=(Transform& other)
{
	return !((*this) == other);
}
