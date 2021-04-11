#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER

#include "base_image_serializer.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

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

		std::string buildBody()
		{
			std::stringstream ss;

			std::vector<Graphics::Color> buffer = this->m_image.toBuffer();

			for (int y = 0; y < m_image.height(); y++)
			{
				for (int x = 0; x < m_image.width(); x++)
				{
					Graphics::Color color = buffer[x + (m_image.width() * y)];
					ss << std::clamp(static_cast<int>(color.red() * 256), 0, 255);
					ss << " ";
					ss << std::clamp(static_cast<int>(color.green() * 256), 0, 255);
					ss << " ";
					ss << std::clamp(static_cast<int>(color.blue() * 256), 0, 255);

					if (x != m_image.width() - 1)
					{
						ss << " ";
					}
				}

				if (y != m_image.height() - 1)
				{
					ss << "\n";
				}
			}

			return ss.str();
		}
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER
