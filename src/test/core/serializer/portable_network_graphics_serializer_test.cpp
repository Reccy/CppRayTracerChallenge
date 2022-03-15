#include "gtest/gtest.h"
#include "serializer/portable_network_graphics_serializer.h"

using namespace CppRayTracerChallenge::Core;

std::vector<unsigned char> decodeResult(std::vector<unsigned char> vec, int start, int length)
{
	return std::vector<unsigned char>(vec.begin() + start, vec.begin() + start + length);
}

TEST(CppRayTracerChallenge_Core_Serializer_PortableNetworkGraphicsSerializer, returns_correct_file_extension)
{
	Serializer::PortableNetworkGraphicsSerializer png;

	std::string expectedResult = png.fileExtension();

	std::string result = std::string("png");

	EXPECT_EQ(result, expectedResult);
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
	Graphics::Image image(98, 98);

	Serializer::PortableNetworkGraphicsSerializer png;
	png.serialize(image);

	std::vector<unsigned char> expectedResult = {
		0, 0, 0, 13,		// Chunk Length: 13
		73, 72, 68, 82,		// Chunk Type: IHDR
		0, 0, 0, 98,		// Width: 98
		0, 0, 0, 98,		// Height: 98
		8,					// Bit Depth: 8
		2,					// Color Mode: 2 (Truecolor)
		0,					// Compression Method: 0 (Default Standard)
		0,					// Filter: 0 (Default Standard)
		0,					// Interlace: 0 (Default Interlace)
		36, 199, 145, 89	// CRC
	};

	std::vector<unsigned char> result = decodeResult(png.buffer(), 8, 25);

	EXPECT_EQ(result, expectedResult);
}
/*
TEST(CppRayTracerChallenge_Core_Serializer_PortableNetworkGraphicsSerializer, sets_correct_idat_chunk)
{
	Graphics::Image image(5, 5);

	Serializer::PortableNetworkGraphicsSerializer png;
	png.serialize(image);

	std::vector<unsigned char> expectedResult = {
		0, 0, 0, 0,			// Chunk Length: 0 (TODO: Set correct expected length)
		73, 68, 65, 84,		// Chunk Type: IDAT
		// DATA START
		0b10000111,			// zlib compression method / flags code
		0b01100010,			// Additional flags / check bits
		0,					// Compressed data blocks
		0, 0, 0, 0,			// zlib check value
		// DATA END
		0, 0, 0, 0			// CRC (TODO)
	};

	std::vector<unsigned char> result = decodeResult(png.buffer(), 26, (int)png.buffer().size() - 12);

	EXPECT_EQ(result, expectedResult);
}
*/
TEST(CppRayTracerChallenge_Core_Serializer_PortableNetworkGraphicsSerializer, sets_correct_iend_chunk)
{
	Graphics::Image image(3, 3);

	Serializer::PortableNetworkGraphicsSerializer png;
	png.serialize(image);

	std::vector<unsigned char> expectedResult = {
		0, 0, 0, 0,			// Chunk Length: 0
		73, 69, 78, 68,		// Chunk Type: IEND
		174, 66, 96, 130	// CRC
	};

	std::vector<unsigned char> result = decodeResult(png.buffer(), (int)png.buffer().size() - 12, 12);

	EXPECT_EQ(result, expectedResult);
}
