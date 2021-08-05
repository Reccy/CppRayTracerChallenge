#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WORLD
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WORLD

#include "../renderer/world.h"
#include "../renderer/camera.h"

namespace CppRayTracerChallenge::Core::Serializer
{
	class WorldSerializer
	{
	public:
		WorldSerializer() :
			m_world()
		{
			m_camera = Renderer::Camera(2, 2, 2);
		}

		void serialize();
		void deserialize(std::vector<char> buffer);
		
		Renderer::World world() const;
		Renderer::Camera camera() const;
	private:
		Renderer::World m_world;
		Renderer::Camera m_camera;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WORLD
