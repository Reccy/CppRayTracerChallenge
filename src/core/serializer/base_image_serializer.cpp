#include "base_image_serializer.h"

namespace CppRayTracerChallenge::Core::Serializer
{
	BaseImageSerializer::BaseImageSerializer() : m_image(CppRayTracerChallenge::Core::Graphics::Image(0, 0)) {};

	CppRayTracerChallenge::Core::Graphics::Image BaseImageSerializer::image() const
	{
		return this->m_image;
	}

	std::vector<unsigned char> BaseImageSerializer::buffer() const
	{
		return this->m_buffer;
	}
}
