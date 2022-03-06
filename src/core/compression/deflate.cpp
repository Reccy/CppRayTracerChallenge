#include "deflate.h"
#include "huffman_coding.h"

using namespace CppRayTracerChallenge::Core::Compression;

DeflateBlock::DeflateBlock(std::vector<unsigned char> data, bool isFinal)
{
	// Set BFINAL
	m_data[0] = isFinal;

	// Set BTYPE to 10 (Compressed with Dynamic Huffman Codes)
	m_data[1] = 1;
	m_data[2] = 0;

	m_writeIndex = 3; // Just after last bit written

	// Generate Huffman Codes
	HuffmanCoding huffman(data);
}

DeflateBlock::DeflateBitset DeflateBlock::data() const
{
	return m_data;
}
