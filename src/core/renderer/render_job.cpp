#include "render_job.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	RenderJob::RenderJob(int xOffset, int yOffset, int width, int height, const World& world, const Camera& camera) :
		m_xOffset(xOffset),
		m_yOffset(yOffset),
		m_width(width),
		m_height(height),
		m_world(world),
		m_camera(camera),
		m_canvas(width, height)
	{};

	bool RenderJob::isComplete() const
	{
		return m_isComplete;
	}

	CppRayTracerChallenge::Core::Graphics::Canvas RenderJob::canvas() const
	{
		return m_canvas;
	}

	void RenderJob::doRender()
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				Math::Ray ray = m_camera.rayForPixel(m_xOffset + x, m_yOffset + y);
				Graphics::Color color = m_world.colorAt(ray);
				m_canvas.writePixel(x, y, color);
			}
		}

		m_isComplete = true;
	}
}
