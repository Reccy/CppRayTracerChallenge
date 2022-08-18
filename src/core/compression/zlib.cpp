#include "zlib.h"
#include <iostream>
#include "../compression/deflate_block.h"

bool inline getBit(CppRayTracerChallenge::Core::Compression::DeflateBlock::DeflateBitset const& bitset, unsigned int index)
{
	if (index >= bitset.size())
		return false;

	return bitset.test(index);
}

unsigned char setWithFCHECK(unsigned char cmf, unsigned char flg)
{
	// Data size is assumed to be 16 bits

	// TODO: Figure out algebraic formula for determining the FCHECK, without doing a for loop

	std::bitset<8> cmfBits = cmf;
	std::bitset<8> flgBits = flg;

	for (unsigned int i = 0b00000; i <= 0b11111; ++i)
	{
		std::bitset<16> checkBits;
		std::bitset<5> indexBits = i;

		// Set the iterated number
		checkBits[0] = indexBits[0];
		checkBits[1] = indexBits[1];
		checkBits[2] = indexBits[2];
		checkBits[3] = indexBits[3];
		checkBits[4] = indexBits[4];

		// Set the flg
		checkBits[5] = flgBits[5]; // FDICT
		checkBits[6] = flgBits[6]; // FLEVEL
		checkBits[7] = flgBits[7]; // FLEVEL

		// Set the cmf
		checkBits[8] = cmfBits[0];
		checkBits[9] = cmfBits[1];
		checkBits[10] = cmfBits[2];
		checkBits[11] = cmfBits[3];
		checkBits[12] = cmfBits[4];
		checkBits[13] = cmfBits[5];
		checkBits[14] = cmfBits[6];
		checkBits[15] = cmfBits[7];

		auto check = static_cast<unsigned int>(checkBits.to_ulong());

		if (check % 31 == 0)
		{
			std::bitset<8> resultBits;

			resultBits[0] = checkBits[0];
			resultBits[1] = checkBits[1];
			resultBits[2] = checkBits[2];
			resultBits[3] = checkBits[3];
			resultBits[4] = checkBits[4];
			resultBits[5] = checkBits[5];
			resultBits[6] = checkBits[6];
			resultBits[7] = checkBits[7];

			return static_cast<unsigned char>(resultBits.to_ulong());
		}
	}

	throw std::logic_error("Could not compute a valid FCHECK when compressing file");
}

std::vector<unsigned char> CppRayTracerChallenge::Core::Compression::ZLIB::compress(std::vector<unsigned char> input)
{
	std::vector<unsigned char> result;

	// zlib compression method/ flags code (CMF)
	// CM (Compression Method) is "deflate" method for PNGs - 1000 (8)
	// CINFO (Compression Info) is "32k window size for LZ77" - 0111 (7)
	// Therefore the zlib compression method / flags code is 10000111 (135)
	//
	// Additional flags / check bits (FLG)
	// FCHECK
	// FDICT - If a dictionary is present after the FLG bit - 0 (0)
	// FLEVEL - Uses default algorithm - 010 (2)
	unsigned char cmf = 0b01111000;
	unsigned char flg = 0b00000000;

	result.push_back(cmf); // zlib compression method/flags code
	result.push_back(setWithFCHECK(cmf, flg));

	auto inputDataSize = input.size();
	auto maxBytes = DeflateBlock::MAX_BYTES;

	unsigned int deflateBlockCount = static_cast<unsigned int>(std::ceil((double)input.size() / (double)DeflateBlock::MAX_BYTES));

	for (unsigned int deflateIndex = 0; deflateIndex < deflateBlockCount; ++deflateIndex)
	{
		auto startIdx = maxBytes * deflateIndex;
		auto endIdx = startIdx + maxBytes;

		bool isFinal = deflateIndex == deflateBlockCount - 1;

		if (isFinal)
			endIdx = static_cast<unsigned int>(inputDataSize);

		auto startIt = input.begin() + startIdx;
		auto endIt = input.begin() + endIdx;

		std::vector<unsigned char> dataChunk(endIdx - startIdx);
		std::copy(startIt, endIt, dataChunk.begin());

		auto deflate = DeflateBlock(dataChunk, isFinal);

		auto deflateSize = deflate.size();
		auto const& bitset = deflate.data();

		unsigned int numBytes = static_cast<unsigned int>(ceil(deflateSize / 8));

		// COMPRESSED DATA -> Convert bits to bytes
		for (unsigned int i = 0; i < numBytes; ++i)
		{
			auto offset = i * 8;

			std::bitset<8> bits;

			bits[0] = getBit(bitset, 0 + offset);
			bits[1] = getBit(bitset, 1 + offset);
			bits[2] = getBit(bitset, 2 + offset);
			bits[3] = getBit(bitset, 3 + offset);
			bits[4] = getBit(bitset, 4 + offset);
			bits[5] = getBit(bitset, 5 + offset);
			bits[6] = getBit(bitset, 6 + offset);
			bits[7] = getBit(bitset, 7 + offset);

			result.push_back(static_cast<unsigned char>(bits.to_ulong()));
		}
	}

	return result;
}
