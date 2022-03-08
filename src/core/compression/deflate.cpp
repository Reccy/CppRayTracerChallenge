#include "deflate.h"
#include "huffman_coding.h"

using namespace CppRayTracerChallenge::Core::Compression;

DeflateBlock::DeflateBlock(std::vector<unsigned char> data, bool isFinal)
{
	// Set BFINAL
	m_data[0] = isFinal;

	// Set BTYPE to 2 (10 LSB first) (Compressed with Dynamic Huffman Codes)
	m_data[1] = 0;
	m_data[2] = 1;

	// Set HLIT -  # of Literal/Length codes (5 bits)
	// Set HDIST - # of Distance codes (5 bits)
	// Set HCLEN - # of Code Length codes (4 bits)
	
	m_writeIndex = 17; // Just after last bit written

	// Generate Huffman Codes
	HuffmanCoding huffman(data);
}

DeflateBlock::DeflateBitset DeflateBlock::data() const
{
	return m_data;
}

void DeflateBlock::write(unsigned char byte)
{
	m_data[m_writeIndex + 0] = (byte >> 0) & 1;
	m_data[m_writeIndex + 1] = (byte >> 1) & 1;
	m_data[m_writeIndex + 2] = (byte >> 2) & 1;
	m_data[m_writeIndex + 3] = (byte >> 3) & 1;
	m_data[m_writeIndex + 4] = (byte >> 4) & 1;
	m_data[m_writeIndex + 5] = (byte >> 5) & 1;
	m_data[m_writeIndex + 6] = (byte >> 6) & 1;
	m_data[m_writeIndex + 7] = (byte >> 7) & 1;

	m_writeIndex += 8;
}
