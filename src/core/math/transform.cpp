#include "transform.h"
#include "RML.h"
#include <vector>

namespace CppRayTracerChallenge::Core::Math
{
	Transform::Transform() : m_matrix(RML::Matrix<double, 4, 4>::identity()) {};

	const Transform Transform::translate(const double x, const double y, const double z) const
	{
		Transform copy = *this;

		RML::Matrix<double, 4, 4> translationMatrix = RML::Matrix<double, 4, 4>::identity();
		translationMatrix(0, 3) = x;
		translationMatrix(1, 3) = y;
		translationMatrix(2, 3) = z;

		copy.m_matrices.push(translationMatrix);

		return copy;
	}

	const Transform Transform::scale(const double x, const double y, const double z) const
	{
		Transform copy = *this;

		RML::Matrix<double, 4, 4> scaleMatrix = RML::Matrix<double, 4, 4>::identity();
		scaleMatrix(0, 0) = x;
		scaleMatrix(1, 1) = y;
		scaleMatrix(2, 2) = z;

		copy.m_matrices.push(scaleMatrix);

		return copy;
	}

	const Transform Transform::rotate(const double x, const double y, const double z) const
	{
		Transform copy = *this;

		const double xRad = RML::Trig::degrees_to_radians(x);
		const double yRad = RML::Trig::degrees_to_radians(y);
		const double zRad = RML::Trig::degrees_to_radians(z);

		RML::Matrix<double, 4, 4> xRotationMatrix = RML::Matrix<double, 4, 4>({
			1, 0, 0, 0,
			0, cos(xRad), -sin(xRad), 0,
			0, sin(xRad), cos(xRad), 0,
			0, 0, 0, 1
			});

		RML::Matrix<double, 4, 4> yRotationMatrix = RML::Matrix<double, 4, 4>({
			cos(yRad), 0, sin(yRad), 0,
			0, 1, 0, 0,
			-sin(yRad), 0, cos(yRad), 0,
			0, 0, 0, 1
			});

		RML::Matrix<double, 4, 4> zRotationMatrix = RML::Matrix<double, 4, 4>({
			cos(zRad), -sin(zRad), 0, 0,
			sin(zRad), cos(zRad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			});

		copy.m_matrices.push(xRotationMatrix);
		copy.m_matrices.push(yRotationMatrix);
		copy.m_matrices.push(zRotationMatrix);

		return copy;
	}

	const Transform Transform::shear(const double xY, const double xZ, const double yX, const double yZ, const double zX, const double zY) const
	{
		Transform copy = *this;

		RML::Matrix<double, 4, 4> shearMatrix = RML::Matrix<double, 4, 4>({
			1, xY, xZ, 0,
			yX, 1, yZ, 0,
			zX, zY, 1, 0,
			0, 0, 0, 1
			});

		copy.m_matrices.push(shearMatrix);

		return copy;
	}

	const Transform Transform::transpose() const
	{
		Transform copy = *this;

		copy.calculateMatrix();

		copy.m_matrix = copy.m_matrix.transpose();

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

	const RML::Matrix<double, 4, 4> Transform::matrix() const
	{
		Transform copy = *this;

		copy.calculateMatrix();

		return copy.m_matrix;
	}

	RML::Tuple4<double> Transform::operator*(const RML::Tuple4<double>& tuple) const
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
}
