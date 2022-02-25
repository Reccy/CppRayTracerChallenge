#include "gtest/gtest.h"
#include "serializer/portable_network_graphics_serializer.h"

using namespace CppRayTracerChallenge::Core;

std::vector<unsigned char> decodeResult(std::vector<unsigned char> vec, int start, int length)
{
	return std::vector<unsigned char>(vec.begin() + start, vec.begin() + start + length);
}

TEST(CppRayTracerChallenge_Core_Serializer_PortableNetworkGraphicsSerializer, sets_correct_signature)
{
	Graphics::Image image(5, 3);

	Serializer::PortableNetworkGraphicsSerializer png;
	png.serialize(image);

	std::vector<unsigned char> expectedResult = { 137, 80, 78, 71, 13, 10, 26, 10 };
	
	std::vector<unsigned char> result = decodeResult(png.buffer(), 0, 8);

	EXPECT_EQ(result, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Serializer_PortableNetworkGraphicsSerializer, sets_correct_ihdr_chunk)
{
	Graphics::Image image(5, 3);

	Serializer::PortableNetworkGraphicsSerializer png;
	png.serialize(image);

	std::vector<unsigned char> expectedResult = {
		0, 0, 0, 13,		// Chunk Length: 13
		73, 72, 68, 82,		// Chunk Type: IHDR
		0, 0, 0, 5,			// Width: 5
		0, 0, 0, 3,			// Height: 3
		8,					// Bit Depth: 8
		2,					// Color Mode: 2 (Truecolor)
		0,					// Compression Method: 0 (Default Standard)
		0,					// Filter: 0 (Default Standard)
		0,					// Interlace: 0 (Default Interlace)
		0, 0, 0, 0			// CRC (NOT IMPLEMENTED YET?)
	};

	std::vector<unsigned char> result = decodeResult(png.buffer(), 8, 25);

	EXPECT_EQ(result, expectedResult);
}
