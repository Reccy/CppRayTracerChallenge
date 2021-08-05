#include "camera.h"
#include "render_job.h"
#include "../graphics/canvas.h"
#include "../math/trig.h"

#include <taskflow/taskflow.hpp>

using namespace CppRayTracerChallenge::Core::Renderer;
using namespace CppRayTracerChallenge::Core::Math;
using namespace CppRayTracerChallenge::Core;

Camera::Camera(int hSize, int vSize, int fieldOfView) :
	m_hSize(hSize), m_vSize(vSize), m_fieldOfView(fieldOfView), m_transformMatrix(Matrix<double, 4, 4>::identity()), m_renderCanvas(std::make_unique<Graphics::Canvas>(m_hSize, m_vSize))
{
	calculatePixelSize();
};

Camera::Camera(const Camera& camera) :
	m_hSize(camera.m_hSize),
	m_vSize(camera.m_vSize),
	m_fieldOfView(camera.m_fieldOfView),
	m_halfWidth(camera.m_halfWidth),
	m_halfHeight(camera.m_halfHeight),
	m_pixelSize(camera.m_pixelSize),
	m_transformMatrix(camera.m_transformMatrix)
{
	m_renderCanvas = std::make_unique<Graphics::Canvas>();
	m_renderCanvas->fromBuffer(camera.m_renderCanvas->toBuffer());
}

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

void Camera::transform(const Math::Matrix<double, 4, 4>& matrix)
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

Matrix<double, 4, 4> Camera::transformMatrix() const
{
	return m_transformMatrix;
}

std::vector<Graphics::Color> composite(const std::vector<std::unique_ptr<RenderJob>>& renderJobs)
{
	std::vector<Graphics::Color> compositeBuffer;

	for (int i = 0; i < renderJobs.size(); ++i)
	{
		const std::vector<Graphics::Color>& renderBuffer = renderJobs[i]->canvas().toBuffer();
		compositeBuffer.insert(std::end(compositeBuffer), std::begin(renderBuffer), std::end(renderBuffer));
	}

	return compositeBuffer;
}

bool jobsComplete(const std::vector<std::unique_ptr<RenderJob>>& renderJobs)
{
	for (int i = 0; i < renderJobs.size(); ++i)
	{
		if (!renderJobs[i]->isComplete())
		{
			return false;
		}
	}

	return true;
}

Graphics::Image Camera::render(const World& world, unsigned int threads = 1) const
{
	tf::Executor executor(threads);
	tf::Taskflow taskflow;

	std::vector<std::unique_ptr<RenderJob>> renderJobs;

	for (int y = 0; y < m_vSize; ++y)
	{
		renderJobs.push_back(std::make_unique<RenderJob>(0, y , m_hSize, 1, world, *this));
	}

	for (int y = 0; y < m_vSize; ++y)
	{
		auto& job = renderJobs[y];

		taskflow.emplace([&]() { job->doRender(); });
	}

	executor.run(taskflow);

	while (!jobsComplete(renderJobs))
	{
		m_renderCanvas->fromBuffer(composite(renderJobs));
	}

	m_renderCanvas->fromBuffer(composite(renderJobs));

	return renderedImage();
}

Graphics::Image Camera::renderedImage() const
{
	Graphics::Image image = Graphics::Image(m_hSize, m_vSize, m_renderCanvas->toBuffer());
	return image;
}

Matrix<double, 4, 4> Camera::viewMatrix(const Point from, const Point to, const Vector up)
{
	Vector forward = (to - from).normalize();
	Vector upNormalized = up.normalize();
	Vector left = Vector::cross(forward, upNormalized);
	Vector trueUp = Vector::cross(left, forward);

	Matrix<double, 4, 4> orientation = Matrix<double, 4, 4>({
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
