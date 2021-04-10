#include "portable_pixmap_image_serializer.h"

using namespace CppRayTracerChallenge::Core::Serializer;

void PortablePixmapImageSerializer::serialize(Graphics::Image image)
{
	this->m_image = image;

	std::stringstream data;
	data << buildHeader();

	std::string datastr = data.str();
	this->m_buffer = std::vector<char>(datastr.begin(), datastr.end());
}

void PortablePixmapImageSerializer::deserialize(std::vector<char> buffer)
{
	this->m_buffer = buffer;

	// TODO: Implementation
}
