#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_CAMERA_DESERIALIZER
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_CAMERA_DESERIALIZER

#include <string>
#include "../renderer/camera.h"

namespace CppRayTracerChallenge::Core::Serializer
{
	/// <summary>
	/// Loads a camera definition and creates a Camera object
	/// </summary>
	class CameraDeserializer
	{
	public:
		
		/// <param name="buffer">Buffer containing valid YAML to load</param>
		CameraDeserializer(const std::string buffer);

		const std::shared_ptr<Renderer::Camera> camera() const;
	private:
		std::shared_ptr<Renderer::Camera> m_camera;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_CAMERA_DESERIALIZER