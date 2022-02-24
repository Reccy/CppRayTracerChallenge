#include "portable_network_graphics_serializer.h"

using namespace CppRayTracerChallenge::Core::Serializer;

void PortableNetworkGraphicsSerializer::serialize(Graphics::Image image)
{
	this->m_image = image;

	std::stringstream data;
	data << buildSignature();

	std::string datastr = data.str();
	this->m_buffer = std::vector<char>(datastr.begin(), datastr.end());
}

void PortableNetworkGraphicsSerializer::deserialize(std::vector<char> buffer)
{
	this->m_buffer = buffer;

	// TODO: Implementation
}

std::string PortableNetworkGraphicsSerializer::buildSignature()
{
	std::stringstream ss;

	ss << (unsigned char)137;
	ss << (unsigned char)80;
	ss << (unsigned char)78;
	ss << (unsigned char)71;
	ss << (unsigned char)13;
	ss << (unsigned char)10;
	ss << (unsigned char)26;
	ss << (unsigned char)10;

	return ss.str();
}
