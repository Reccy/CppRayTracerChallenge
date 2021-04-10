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
		header << str << "\n";
		count++;
	}

	EXPECT_EQ(header.str(), expectedResult);
}
