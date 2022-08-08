#pragma once
#include <vector>

namespace CppRayTracerChallenge::Core::Compression::ZLIB
{
	std::vector<unsigned char> compress(std::vector<unsigned char> input);
}
