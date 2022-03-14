#ifndef _CPPRAYTRACERCHALLENGE_CORE_ENCRYPTION_CRC
#define _CPPRAYTRACERCHALLENGE_CORE_ENCRYPTION_CRC

#include <vector>

namespace CppRayTracerChallenge::Core::Encryption::Adler32
{
	unsigned int run(std::vector<unsigned char> data);
}

#endif _CPPRAYTRACERCHALLENGE_CORE_ENCRYPTION_CRC
