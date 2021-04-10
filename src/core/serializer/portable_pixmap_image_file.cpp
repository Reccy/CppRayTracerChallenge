#include "portable_pixmap_image_file.h"

using namespace CppRayTracerChallenge::Core::Serializer;

std::stringstream PortablePixmapImageFile::data() const
{
	std::stringstream ss(this->m_data.str());
	return ss;
}
