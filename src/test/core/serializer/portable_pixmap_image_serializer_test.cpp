#include "gtest/gtest.h"
#include "serializer/portable_pixmap_image_serializer.h"

#include <vector>

using namespace CppRayTracerChallenge::Core::Serializer;

TEST(CppRayTracerChallenge_Core_Serializer_PortablePixmapImageSerializer, sets_correct_header)
{
	Graphics::Image image(5,3);

	PortablePixmapImageSerializer ppm;
	ppm.serialize(image);

	std::string expectedResult = "";
	expectedResult += "P3\n";
	expectedResult += "5 3\n";
	expectedResult += "255\n";

	std::vector<char> chars = ppm.buffer();

	std::stringstream data(std::string(chars.begin(), chars.end()));
	std::stringstream header;

	// Read the first 3 lines of the header from the PPM file
	std::string str;
	int count = 0;
	while (getline(data, str) && count < 3)
	{
		header << str;
		if (data.peek() != EOF)
		{
			header << "\n";
		}

		count++;
	}

	EXPECT_EQ(header.str(), expectedResult);
}

TEST(CppRayTracerChallenge_Core_Serializer_PortablePixmapImageSerializer, sets_correct_image_data)
{
	// Setup color palette
	Graphics::Color c1 = Graphics::Color(1.5, 0, 0);
	Graphics::Color c2 = Graphics::Color(0, 0.5, 0);
	Graphics::Color c3 = Graphics::Color(-0.5, 0, 1);
	Graphics::Color b = Graphics::Color::black();

	// Setup image
	std::vector<Graphics::Color> colors {
		c1, b, b, b, b,
		b, b, c2, b, b,
		b, b, b, b, c3,
	};

	Graphics::Image image(5, 3, colors);

	// Setup expected result
	std::stringstream ss;
	ss << "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
	ss << "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0\n";
	ss << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255";

	std::string expectedResult = ss.str();

	// Get actual result
	PortablePixmapImageSerializer ppm;
	ppm.serialize(image);

	std::vector<char> chars = ppm.buffer();
	std::stringstream data(std::string(chars.begin(), chars.end()));
	std::stringstream body;

	std::string str;
	int count = 0;
	while (getline(data, str) && count < 6)
	{
		if (count < 3) {
			count++;
			continue;
		}

		body << str;

		if (data.peek() != EOF)
		{
			body << "\n";
		}

		count++;
	}

	std::string result = body.str();

	EXPECT_EQ(result, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Serializer_PortablePixmapImageSerializer, split_long_lines_over_70_chars)
{
	// Setup color palette
	Graphics::Color c = Graphics::Color(1.0f, 0.8f, 0.6f);

	// Setup image
	Graphics::Image image = Graphics::Image(10, 2, c);

	// Setup expected result
	std::stringstream ss;
	ss << "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n";
	ss << "153 255 204 153 255 204 153 255 204 153 255 204 153\n";
	ss << "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n";
	ss << "153 255 204 153 255 204 153 255 204 153 255 204 153";

	std::string expectedResult = ss.str();

	// Get actual result
	PortablePixmapImageSerializer ppm;
	ppm.serialize(image);

	std::vector<char> chars = ppm.buffer();
	std::stringstream data(std::string(chars.begin(), chars.end()));
	std::stringstream body;

	std::string str;
	int count = 0;
	while (getline(data, str) && count < 7)
	{
		if (count < 3) {
			count++;
			continue;
		}

		body << str;

		if (data.peek() != EOF)
		{
			body << "\n";
		}

		count++;
	}

	std::string result = body.str();

	EXPECT_EQ(result, expectedResult);
}
