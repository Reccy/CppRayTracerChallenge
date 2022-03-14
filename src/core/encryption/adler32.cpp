#include "adler32.h"

namespace CppRayTracerChallenge::Core::Encryption::Adler32
{
	unsigned int run(std::vector<unsigned char> data)
	{
		unsigned int a = 1;
		unsigned int b = 0;

		for (unsigned int index = 0; index < data.size(); ++index)
		{
			a = (a + data[index]) % 65521;
			b = (b + a) % 65521;
		}

		return (b << 16) | a;
	}
}
