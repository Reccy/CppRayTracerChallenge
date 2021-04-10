#include "gtest/gtest.h"
#include "serializer/portable_pixmap_image_file.h"

#include <vector>

using namespace CppRayTracerChallenge::Core::Serializer;

TEST(CppRayTracerChallenge_Core_Serializer_PortablePixmapImageFile, sets_correct_header)
{
	std::vector<char> payload;
	PortablePixmapImageFile ppm(5, 3, payload);

	std::string expectedResult = "";
	expectedResult += "P3\n";
	expectedResult += "5 3\n";
	expectedResult += "255\n";

	std::stringstream data = ppm.data();
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
