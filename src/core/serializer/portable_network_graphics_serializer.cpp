#include "portable_network_graphics_serializer.h"

using namespace CppRayTracerChallenge::Core::Serializer;

void PortableNetworkGraphicsSerializer::serialize(Graphics::Image image)
{
	this->m_image = image;

	// TODO: Implementation
}

void PortableNetworkGraphicsSerializer::deserialize(std::vector<char> buffer)
{
	this->m_buffer = buffer;

	// TODO: Implementation
}
