#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER

#include "base_image_serializer.h"

#include <fstream>
#include <sstream>
#include <vector>

namespace CppRayTracerChallenge::Core::Serializer
{
	/// <summary>
	/// A PortablePixmap serializer responsible for serializing an image to a buffer and vice versa
	/// </summary>
	class PortablePixmapImageSerializer : public BaseImageSerializer
	{
	public:
		void serialize(Graphics::Image image) override;
		void deserialize(std::vector<char> buffer) override;
	private:
		std::string buildHeader()
		{
			std::stringstream ss;

			ss << "P3\n";
			ss << std::to_string(this->m_image.width()) << " " << std::to_string(this->m_image.height()) << "\n";
			ss << "255\n";

			return ss.str();
		};
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER
