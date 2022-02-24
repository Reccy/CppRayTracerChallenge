#include "gtest/gtest.h"
#include "serializer/portable_network_graphics_serializer.h"

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Serializer_PortableNetworkGraphicsSerializer, sets_correct_signature)
{
	Graphics::Image image(5, 3);

	Serializer::PortableNetworkGraphicsSerializer png;
	png.serialize(image);

	std::vector<unsigned char> expectedResult = { 137, 80, 78, 71, 13, 10, 26, 10 };
	
	std::vector<unsigned char> chars = std::vector<unsigned char>(png.buffer());
	std::vector<unsigned char> result = std::vector(chars.begin(), chars.begin() + 8);

	EXPECT_EQ(result, expectedResult);
}
