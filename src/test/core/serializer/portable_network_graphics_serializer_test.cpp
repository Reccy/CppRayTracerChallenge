#include "gtest/gtest.h"
#include "serializer/portable_network_graphics_serializer.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Serializer_PortableNetworkGraphicsSerializer, sets_correct_signature)
{
	Graphics::Image image(5, 3);

	Serializer::PortableNetworkGraphicsSerializer png;
	png.serialize(image);

	std::stringstream expectedResultStream;
	expectedResultStream << (unsigned char)137;
	expectedResultStream << (unsigned char)80;
	expectedResultStream << (unsigned char)78;
	expectedResultStream << (unsigned char)71;
	expectedResultStream << (unsigned char)13;
	expectedResultStream << (unsigned char)10;
	expectedResultStream << (unsigned char)26;
	expectedResultStream << (unsigned char)10;

	std::string expectedResult = expectedResultStream.str();

	std::vector<char> chars = png.buffer();

	std::stringstream data(std::string(chars.begin(), chars.end()));
	std::stringstream signature;

	// Read the first 8 bytes from the PNG file
	std::string str = data.str();

	for (int i = 0; i < 8; ++i)
	{
		signature << str[i];
	}

	EXPECT_EQ(signature.str(), expectedResult);
}
