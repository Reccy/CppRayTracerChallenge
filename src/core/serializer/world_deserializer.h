#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WORLD_DESERIALIZER
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WORLD_DESERIALIZER

#include <string>
#include "../renderer/world.h"

namespace CppRayTracerChallenge::Core::Serializer
{
	/// <summary>
	/// Loads a world definition and creates a World object
	/// </summary>
	class WorldDeserializer
	{
	public:
		/// <param name="buffer">Buffer containing valid YAML to load</param>
		WorldDeserializer(const std::string buffer);

		const std::shared_ptr<Renderer::World> world() const;
	private:
		std::shared_ptr<Renderer::World> m_world;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WORLD_DESERIALIZER