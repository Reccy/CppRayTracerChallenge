#include <gtest/gtest.h>
#include <serializer/world_deserializer.h>
#include <string>
#include <fstream>

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Serializer_WorldDeserializer, basic_test) // todo: rewrite to actual test. this is proof of concept
{
	// Load example file
	std::ifstream ymlfile("./world_deserializer_test_file.yml");
	const std::string ymlstr((std::istreambuf_iterator<char>(ymlfile)),
		(std::istreambuf_iterator<char>()));

	Serializer::WorldDeserializer deserializer(ymlstr);

	EXPECT_EQ(deserializer.world()->lightCount(), 1);
	EXPECT_EQ(deserializer.world()->objectCount(), 2);
}