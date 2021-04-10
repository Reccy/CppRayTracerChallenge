#include "base_image_serializer.h"

using namespace CppRayTracerChallenge::Core::Serializer;

CppRayTracerChallenge::Core::Graphics::Image BaseImageSerializer::image() const
{
	return this->m_image;
}

std::vector<char> BaseImageSerializer::buffer() const
{
	return this->m_buffer;
}
