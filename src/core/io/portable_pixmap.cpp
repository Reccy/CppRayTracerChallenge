#include "portable_pixmap.h"

using namespace CppRayTracerChallenge::Core::Io;

std::stringstream PortablePixmap::data() const
{
	std::stringstream ss(this->m_data.str());
	return ss;
}
