#include <gtest/gtest.h>
#include "encryption/crc.h"

using namespace CppRayTracerChallenge::Core::Encryption;

TEST(CppRayTracerChallenge_Core_Encryption_CRC, returns_expected_result)
{
	std::vector<unsigned char> bytes = { 73, 69, 78, 68 };

	std::vector<unsigned char> expectedResult = { 174, 66, 96, 130 };

	std::vector<unsigned char> result = CRC::run(bytes);

	EXPECT_EQ(result, expectedResult);
}

TEST(CppRayTracerChallenge_Core_Encryption_CRC, multiple_runs_return_same_result)
{
	std::vector<unsigned char> bytes = { 73, 69, 78, 68 };

	std::vector<unsigned char> expectedResult = { 174, 66, 96, 130 };

	std::vector<unsigned char> result0 = CRC::run(bytes);
	std::vector<unsigned char> result1 = CRC::run(bytes);

	EXPECT_EQ(result0, expectedResult);
	EXPECT_EQ(result1, expectedResult);
}
