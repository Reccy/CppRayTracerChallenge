#include "portable_pixmap_image_serializer.h"

using namespace CppRayTracerChallenge::Core::Serializer;

void PortablePixmapImageSerializer::serialize(Graphics::Image image)
{
	this->m_image = image;

	std::stringstream data;
	data << buildHeader();
	data << buildBody();

	std::string datastr = data.str();
	this->m_buffer = std::vector<unsigned char>(datastr.begin(), datastr.end());
}

std::string PortablePixmapImageSerializer::fileExtension() const
{
	return std::string("ppm");
}

std::string PortablePixmapImageSerializer::buildHeader()
{
	std::stringstream ss;

	ss << "P3\n";
	ss << std::to_string(this->m_image.width()) << " " << std::to_string(this->m_image.height()) << "\n";
	ss << "255\n";

	return ss.str();
}

std::string PortablePixmapImageSerializer::buildBody()
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

std::string PortablePixmapImageSerializer::convertColorValue(float colorValue)
{
	float scaledColor = colorValue * 255;
	scaledColor = std::ceil(scaledColor);
	scaledColor = std::clamp(scaledColor, 0.0f, 255.0f);

	return std::to_string((int)scaledColor);
}
