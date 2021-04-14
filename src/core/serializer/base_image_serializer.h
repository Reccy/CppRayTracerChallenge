#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_BASE_IMAGE_SERIALIZER
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_BASE_IMAGE_SERIALIZER

#include <fstream>
#include "../graphics/image.h"

namespace CppRayTracerChallenge::Core::Serializer {
	/// <summary>
	/// Base serializer class responsible for serializing an image to a buffer and vice versa
	/// </summary>
	class BaseImageSerializer {
	public:
		BaseImageSerializer();

		/// <summary>
		/// Serializes an Image, making it available to access via the buffer
		/// </summary>
		virtual void serialize(Graphics::Image image) = 0;

		/// <summary>
		/// Deserializes a buffer, converting it into an Image
		/// </summary>
		virtual void deserialize(std::vector<char> buffer) = 0;

		/// <summary>
		/// Returns the deserialized image
		/// </summary>
		CppRayTracerChallenge::Core::Graphics::Image image() const;

		/// <summary>
		/// Returns the serialized image as a buffer
		/// </summary>
		std::vector<char> buffer() const;
	protected:
		Graphics::Image m_image;
		std::vector<char> m_buffer;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_BASE_IMAGE_SERIALIZER
