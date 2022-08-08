#pragma once
#include <vector>
#include <bitset>
#include <memory>

namespace CppRayTracerChallenge::Core::Compression
{
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
