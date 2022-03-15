#include <gtest/gtest.h>
#include "compression/zlib.h"

using namespace CppRayTracerChallenge::Core::Compression;

std::vector<unsigned char> getInput()
{
	return std::vector<unsigned char> { 'h', 'e', 'l', 'l', 'o' };
}

TEST(CppRayTracerChallenge_Core_Compression_ZLIB, sets_cmf_header)
{
	auto input = getInput();

	auto result = ZLIB::compress(input);

	EXPECT_EQ(result[0], 0b01111000);
}

TEST(CppRayTracerChallenge_Core_Compression_ZLIB, sets_flg)
{
	auto input = getInput();

	auto result = ZLIB::compress(input);

	EXPECT_EQ(result[1], 0b00000001);
}

TEST(CppRayTracerChallenge_Core_Compression_ZLIB, compresses_to_expected_result)
{
	auto input = getInput();

	auto result = ZLIB::compress(input);

	auto expectedResult = std::vector<unsigned char>{
		0x78,
		0x1,
		0x1,
		0x5,
		0x0,
		0xFA,
		0xFF,
		'h',
		'e',
		'l',
		'l',
		'o'
	};

	EXPECT_EQ(result, expectedResult);
}
