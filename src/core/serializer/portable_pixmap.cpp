#include "portable_pixmap.h"

using namespace CppRayTracerChallenge::Core::Serializer;

std::stringstream PortablePixmap::data() const
{
	std::stringstream ss(this->m_data.str());
	return ss;
}
