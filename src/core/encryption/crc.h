#pragma once
#include <vector>

/*
* Code Adapted from: w3.org
* Source: https://www.w3.org/TR/PNG/#D-CRCAppendix
*/

namespace CppRayTracerChallenge::Core::Encryption::CRC
{
	// Return the CRC of the passed in bytes
	std::vector<unsigned char> run(std::vector<unsigned char> vec);
}
