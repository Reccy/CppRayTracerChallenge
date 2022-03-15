#ifndef _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_ZLIB
#define _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_ZLIB

#include <vector>

namespace CppRayTracerChallenge::Core::Compression::ZLIB
{
	std::vector<unsigned char> compress(std::vector<unsigned char> input);
}

#endif _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_ZLIB
