#include <gtest/gtest.h>
#include "encryption/adler32.h"

using namespace CppRayTracerChallenge::Core::Encryption;

TEST(CppRayTracerChallenge_Core_Encryption_Adler32, returns_expected_result)
{
	std::vector<unsigned char> bytes = { 'h', 'e', 'l', 'l', 'o' };

	unsigned int expectedResult = 0x062C0215;

	unsigned int result = Adler32::run(bytes);

	EXPECT_EQ(expectedResult, result);
}
