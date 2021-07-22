#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WAVEFRONT_OBJ_SERIALIZER
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WAVEFRONT_OBJ_SERIALIZER

#include <vector>
#include "../math/point.h"
#include "../math/triangle.h"
#include "../renderer/group.h"

namespace CppRayTracerChallenge::Core::Serializer
{
	class WavefrontOBJSerializer
	{
	public:
		void serialize();

		void deserialize(std::vector<char> buffer);

		/// <summary>
		/// Returns the serialized group as a buffer
		/// </summary>
		/// <returns>Group serialized as buffer</returns>
		std::vector<char> buffer() const;

		/// <summary>
		/// Returns the default group
		/// </summary>
		/// <returns>Default group</returns>
		Renderer::Group defaultGroup() const;

		/// <summary>
		/// Returns the list of vertices
		/// </summary>
		/// <returns></returns>
		std::vector<Math::Point> vertices() const;

		/// <summary>
		/// Returns the amount of lines that were ignored during deserialization
		/// </summary>
		/// <returns>Lines ignored during deserialization</returns>
		int ignoredLines() const;
	private:
		std::vector<char> m_buffer;
		std::vector<Math::Point> m_vertices;
		Renderer::Group m_group;
		int m_ignoredLines = 0;

		void parseVertex(std::string line);
		void parseFace(std::string line);
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_WAVEFRONT_OBJ_SERIALIZER
