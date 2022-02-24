#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PNG_IMAGE_SERIALIZER
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PNG_IMAGE_SERIALIZER

#include "base_image_serializer.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

namespace CppRayTracerChallenge::Core::Serializer
{
	/// <summary>
	/// A PortableNetworkGraphics serializer responsible for serializing an image to a buffer and vice versa
	/// </summary>
	class PortableNetworkGraphicsSerializer : public BaseImageSerializer
	{
	public:
		void serialize(Graphics::Image image) override;
		void deserialize(std::vector<unsigned char> buffer) override;
	private:
		std::string buildSignature();
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PNG_IMAGE_SERIALIZER
