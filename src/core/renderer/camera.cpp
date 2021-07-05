#include "camera.h"
#include "render_job.h"
#include "../graphics/canvas.h"
#include "../math/trig.h"

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core;

Camera::Camera(int hSize, int vSize, int fieldOfView) :
	m_hSize(hSize), m_vSize(vSize), m_fieldOfView(fieldOfView), m_transformMatrix(Matrix<double>::identity(4)), m_renderCanvas(std::make_unique<Graphics::Canvas>(m_hSize, m_vSize))
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

void Camera::transform(const Math::Matrix<double>& matrix)
{
	m_transformMatrix = matrix;
}

void Camera::transform(const Math::Transform& transform)
{
	m_transformMatrix = transform.matrix();
}

Ray Camera::rayForPixel(int xPixel, int yPixel) const
{
	// Calculates the offsets from the edge of the canvas to the pixel's center
	double xOffset = static_cast<double>(xPixel + 0.5) * m_pixelSize;
	double yOffset = static_cast<double>(yPixel + 0.5) * m_pixelSize;

	// The positions of the pixels in world space before being transformed
	double worldX = m_halfWidth - xOffset;
	double worldY = m_halfHeight - yOffset;

	// Transform the canvas point and origin using camera matrix to compute ray direction vector
	Point pixel = m_transformMatrix.invert() * Point(worldX, worldY, -1);
	Point origin = m_transformMatrix.invert() * Point(0, 0, 0);
	Vector direction = (pixel - origin).normalize();

	return Ray(origin, direction);
}

Matrix<double> Camera::transformMatrix() const
{
	return m_transformMatrix;
}

std::vector<Graphics::Color> composite(const RenderJob& a, const RenderJob& b)
{
	std::vector<Graphics::Color> compositeBuffer = a.canvas().toBuffer();

	const std::vector<Graphics::Color>& bottomBuffer = b.canvas().toBuffer();

	compositeBuffer.insert(std::end(compositeBuffer), std::begin(bottomBuffer), std::end(bottomBuffer));
	return compositeBuffer;
}

Graphics::Image Camera::render(const World& world) const
{
	RenderJob renderJobTop = RenderJob(0, 0, m_hSize, m_vSize/2, world, *this);
	RenderJob renderJobBottom = RenderJob(0, m_vSize/2, m_hSize, m_vSize/2 + 1, world, *this);

	while (!renderJobTop.isComplete() || !renderJobBottom.isComplete())
	{
		m_renderCanvas->fromBuffer(composite(renderJobTop, renderJobBottom));
	}

	m_renderCanvas->fromBuffer(composite(renderJobTop, renderJobBottom));

	return renderedImage();
}

Graphics::Image Camera::renderedImage() const
{
	Graphics::Image image = Graphics::Image(m_hSize, m_vSize, m_renderCanvas->toBuffer());
	return image;
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

	if (aspect >= 1)
	{
		m_halfWidth = halfView;
		m_halfHeight = halfView / aspect;
	}
	else
	{
		m_halfWidth = halfView * aspect;
		m_halfHeight = halfView;
	}

	m_pixelSize = (m_halfWidth * 2) / m_hSize;
}
