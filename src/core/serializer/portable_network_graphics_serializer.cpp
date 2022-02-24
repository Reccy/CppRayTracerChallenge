#include "portable_network_graphics_serializer.h"

using namespace CppRayTracerChallenge::Core::Serializer;

void PortableNetworkGraphicsSerializer::serialize(Graphics::Image image)
{
	this->m_image = image;

	std::stringstream data;
	data << buildSignature();

	std::string datastr = data.str();
	this->m_buffer = std::vector<unsigned char>(datastr.begin(), datastr.end());
}

void PortableNetworkGraphicsSerializer::deserialize(std::vector<unsigned char> buffer)
{
	this->m_buffer = buffer;

	// TODO: Implementation
}

std::string PortableNetworkGraphicsSerializer::buildSignature()
{
	const unsigned char signature[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

	std::stringstream ss;
	ss << signature;

	return ss.str();
}
