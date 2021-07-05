#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_RENDER_JOB
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_RENDER_JOB

#include <thread>
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

		~RenderJob();

		bool isComplete() const;

		Graphics::Canvas canvas() const;
	private:
		int m_xOffset;
		int m_yOffset;
		int m_width;
		int m_height;
		const World& m_world;
		const Camera& m_camera;
		Graphics::Canvas m_canvas;
		std::thread m_thread;
		bool m_isComplete = false;

		void doRender();
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_RENDER_JOB