#include <gtest/gtest.h>
#include "encryption/adler32.h"

namespace CppRayTracerChallenge::Core::Encryption
{
	TEST(CppRayTracerChallenge_Core_Encryption_Adler32, returns_expected_result_0)
	{
		std::vector<unsigned char> bytes = { 'h', 'e', 'l', 'l', 'o' };

		unsigned int expectedResult = 0x062C0215;

		unsigned int result = Adler32::run(bytes);

		EXPECT_EQ(expectedResult, result);
	}

	TEST(CppRayTracerChallenge_Core_Encryption_Adler32, returns_expected_result_1)
	{
		std::vector<unsigned char> bytes = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

		unsigned int expectedResult = 0x091E01DE;

		unsigned int result = Adler32::run(bytes);

		EXPECT_EQ(expectedResult, result);
	}
}
