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

		DeflateBlock(std::vector<unsigned char> data, bool isFinal, bool isCompressed = true);
		DeflateBitset data() const;
	private:
		DeflateBitset m_data;
		void writeByte(unsigned char byte);
		void writeBit(bool bit);
		void writeShort(unsigned short integer);
		unsigned int m_writeIndex;
		bool m_isCompressed;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_DEFLATE
