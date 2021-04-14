#ifndef _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER
#define _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER

#include "base_image_serializer.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

namespace CppRayTracerChallenge::Core::Serializer
{
	/// <summary>
	/// A PortablePixmap serializer responsible for serializing an image to a buffer and vice versa
	/// http://netpbm.sourceforge.net/doc/ppm.html
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

		std::string convertColorValue(float colorValue)
		{
			float scaledColor = colorValue * 255;
			scaledColor = std::ceil(scaledColor);
			scaledColor = std::clamp(scaledColor, 0.0f, 255.0f);

			return std::to_string((int)scaledColor);
		}

		std::string buildBody()
		{
			std::stringstream ss;

			std::vector<Graphics::Color> buffer = m_image.toBuffer();

			int extendedWidth = m_image.width() * 3;

			std::vector<std::string> values;
			values.reserve(extendedWidth * m_image.height());

			for (int i = 0; i < buffer.size(); i++)
			{
				Graphics::Color color = buffer[i];

				values.push_back(convertColorValue(color.red()));
				values.push_back(convertColorValue(color.green()));
				values.push_back(convertColorValue(color.blue()));
			}

			for (int y = 0; y < m_image.height(); y++)
			{
				size_t charsWritten = 0;

				for (int x = 0; x < extendedWidth; x++)
				{
					int valuesIndex = x + extendedWidth * y;

					std::string value = values[valuesIndex];
					size_t valueSize = value.size();

					ss << value;
					charsWritten += valueSize;

					if (x != extendedWidth - 1)
					{
						std::string nextValue = values[valuesIndex + 1];

						if (charsWritten + nextValue.size() < 70)
						{
							ss << " ";
							++charsWritten;
						}
						else
						{
							ss << "\n";
							charsWritten = 0;
						}
					}
				}

				ss << "\n";
				charsWritten = 0;
			}

			return ss.str();
		}
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER
