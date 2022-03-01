#ifndef _CPPRAYTRACERCHALLENGE_CORE_ENCRYPTION_CRC
#define _CPPRAYTRACERCHALLENGE_CORE_ENCRYPTION_CRC

#include <vector>

/*
* Code Adapted from: w3.org
* Source: https://www.w3.org/TR/PNG/#D-CRCAppendix
*/

namespace CppRayTracerChallenge::Core::Encryption::CRC
{
	// PRIVATE
	void m_makeCrcTable(void);
	
	// PRIVATE
	unsigned long m_updateCrc(unsigned long crc, unsigned char* buf, int len);

	// PRIVATE
	std::vector<unsigned char> m_longToBytes(unsigned long value);

	// Return the CRC of the passed in bytes
	std::vector<unsigned char> run(std::vector<unsigned char> vec);
}

#endif _CPPRAYTRACERCHALLENGE_CORE_ENCRYPTION_CRC
