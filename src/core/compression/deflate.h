#ifndef _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_DEFLATE
#define _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_DEFLATE

#include "huffman_coding.h"
#include <vector>
#include <bitset>

namespace CppRayTracerChallenge::Core::Compression
{
	class Deflate
	{

	};

	class DeflateBlock
	{
	public:
		using DeflateBitset = std::bitset<32768>;

		DeflateBlock(std::vector<unsigned char> data, bool isFinal);
		DeflateBitset data() const;
	private:
		DeflateBitset m_data;
		void write(unsigned char byte);
		unsigned int m_writeIndex;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_DEFLATE
