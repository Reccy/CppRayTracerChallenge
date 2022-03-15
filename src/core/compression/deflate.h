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
		static const unsigned int MAX_BYTES = 65530;
		using DeflateBitset = std::bitset<(MAX_BYTES * 8) + (5 * 8)>;

		DeflateBlock(std::vector<unsigned char> data, bool isFinal);
		const DeflateBitset& data() const;
		unsigned int size() const;
	private:
		std::unique_ptr<DeflateBitset> m_data;
		void writeByte(unsigned char byte);
		void writeBit(bool bit);
		void writeShort(unsigned short integer);
		void writeInt(unsigned int integer);
		unsigned int m_writeIndex;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_DEFLATE
