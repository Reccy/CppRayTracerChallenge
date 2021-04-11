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

			int scaledColorInt = (int)scaledColor;

			std::stringstream ss;
			ss << scaledColorInt;
			return ss.str();
		}

		std::string buildBody()
		{
			std::stringstream body;

			std::vector<Graphics::Color> buffer = this->m_image.toBuffer();

			for (int y = 0; y < m_image.height(); y++)
			{
				std::stringstream row;
				for (int x = 0; x < m_image.width(); x++)
				{
					int bufferIndex = x + (m_image.width() * y);
					Graphics::Color color = buffer[bufferIndex];
					std::vector<std::string> values{
						convertColorValue(color.red()),
						convertColorValue(color.green()),
						convertColorValue(color.blue())
					};

					for (int i = 0; i < values.size(); i++)
					{
						if (i == 2 && x == m_image.width() - 1)
						{
							// if we are at the end of a raster line
							row << values[i];
							row << "\n";
							body << row.str();
							row.str("");
							row.clear();
						}
						else if (row.str().size() + values[i].size() > 70)
						{
							// if we are about to overflow the 70 char limit
							row.seekp(-1, std::ios_base::end);
							row << "\n";
							body << row.str();
							row.str("");
							row.clear();
							row << values[i];
							row << " ";
						}
						else
						{
							row << values[i];
							row << " ";
						}
					}
				}
			}

			return body.str();
		}
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_SERIALIZER_PORTABLE_PIXMAP_IMAGE_SERIALIZER
