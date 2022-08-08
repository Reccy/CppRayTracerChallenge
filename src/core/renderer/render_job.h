#pragma once
#include "world.h"
#include "camera.h"
#include "../graphics/canvas.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class RenderJob
	{
	public:
		RenderJob() = delete;

		RenderJob(int xOffset, int yOffset, int width, int height, const World& world, const Camera& camera);

		bool isComplete() const;

		Graphics::Canvas canvas() const;
		
		void doRender();
	private:
		int m_xOffset;
		int m_yOffset;
		int m_width;
		int m_height;
		const World& m_world;
		const Camera& m_camera;
		Graphics::Canvas m_canvas;
		bool m_isComplete = false;
	};
}
