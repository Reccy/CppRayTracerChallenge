#include <gtest/gtest.h>
#include <serializer/camera_deserializer.h>
#include <fstream>

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Serializer_CameraDeserializer, basic_test) // todo: rewrite
{
	// Load example file
	std::ifstream ymlfile("./world_deserializer_test_file.yml");
	const std::string ymlstr((std::istreambuf_iterator<char>(ymlfile)),
		(std::istreambuf_iterator<char>()));

	Serializer::CameraDeserializer deserializer(ymlstr);

	// quick and dirty test
	EXPECT_EQ(deserializer.camera()->fieldOfView(), 70);
	EXPECT_EQ(deserializer.camera()->resolutionWidth(), 1920);
	EXPECT_EQ(deserializer.camera()->resolutionHeight(), 1080);
}
