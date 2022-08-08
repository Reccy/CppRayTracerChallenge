#pragma once
#include <vector>

namespace CppRayTracerChallenge::Core::Encryption::Adler32
{
	unsigned int run(std::vector<unsigned char> data);
}
