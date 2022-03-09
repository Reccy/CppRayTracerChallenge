#include "deflate.h"
#include "huffman_coding.h"
#include <iostream>

using namespace CppRayTracerChallenge::Core::Compression;

DeflateBlock::DeflateBlock(std::vector<unsigned char> data, bool isFinal, bool isCompressed)
	: m_isCompressed(isCompressed), m_writeIndex(0), m_data(std::make_unique<DeflateBitset>())
{
	unsigned short lengthBytes = static_cast<unsigned short>(data.size());

	if (lengthBytes >= 65536)
	{
		std::string error("Cannot create DEFLATE Block with more than 65536 bytes!");
		std::cerr << error << std::endl;
		throw std::logic_error(error);
	}

	// Set BFINAL
	writeBit(isFinal);

	if (m_isCompressed)
	{
		// Set BTYPE to 2 (10 LSB first) (Compressed with Dynamic Huffman Codes)
		writeBit(0);
		writeBit(1);

		// Set HLIT -  # of Literal/Length codes (5 bits)
		// Set HDIST - # of Distance codes (5 bits)
		// Set HCLEN - # of Code Length codes (4 bits)
	
		// Generate Huffman Codes
		HuffmanCoding huffman(data);
	}
	else
	{
		// Set BTYPE to 0 (Uncompressed)
		writeBit(0);
		writeBit(1);

		// Add 5 bits of padding to the byte boundary
		for (int i = 0; i < 5; ++i)
		{
			writeBit(0);
		}

		writeShort(lengthBytes);
		writeShort(~lengthBytes); // Intentional use of bitwise NOT

		// Write uncompressed data
		for (auto it = data.begin(); it != data.end(); ++it)
		{
			writeByte(*it);
		}
	}
}

unsigned int DeflateBlock::size() const
{
	return m_writeIndex;
}

const DeflateBlock::DeflateBitset& DeflateBlock::data() const
{
	return *m_data;
}

void DeflateBlock::writeBit(bool bit)
{
	(*m_data)[m_writeIndex] = bit;
	m_writeIndex++;
}

void DeflateBlock::writeByte(unsigned char byte)
{
	(*m_data)[m_writeIndex + 0] = (byte >> 0) & 1;
	(*m_data)[m_writeIndex + 1] = (byte >> 1) & 1;
	(*m_data)[m_writeIndex + 2] = (byte >> 2) & 1;
	(*m_data)[m_writeIndex + 3] = (byte >> 3) & 1;
	(*m_data)[m_writeIndex + 4] = (byte >> 4) & 1;
	(*m_data)[m_writeIndex + 5] = (byte >> 5) & 1;
	(*m_data)[m_writeIndex + 6] = (byte >> 6) & 1;
	(*m_data)[m_writeIndex + 7] = (byte >> 7) & 1;

	m_writeIndex += 8;
}

void DeflateBlock::writeShort(unsigned short integer)
{
	// Assuming the Int16 is already little endian
	(*m_data)[m_writeIndex + 0] = (integer >> 0) & 1;
	(*m_data)[m_writeIndex + 1] = (integer >> 1) & 1;
	(*m_data)[m_writeIndex + 2] = (integer >> 2) & 1;
	(*m_data)[m_writeIndex + 3] = (integer >> 3) & 1;
	(*m_data)[m_writeIndex + 4] = (integer >> 4) & 1;
	(*m_data)[m_writeIndex + 5] = (integer >> 5) & 1;
	(*m_data)[m_writeIndex + 6] = (integer >> 6) & 1;
	(*m_data)[m_writeIndex + 7] = (integer >> 7) & 1;

	(*m_data)[m_writeIndex + 8] = (integer >> 8) & 1;
	(*m_data)[m_writeIndex + 9] = (integer >> 9) & 1;
	(*m_data)[m_writeIndex + 10] = (integer >> 10) & 1;
	(*m_data)[m_writeIndex + 11] = (integer >> 11) & 1;
	(*m_data)[m_writeIndex + 12] = (integer >> 12) & 1;
	(*m_data)[m_writeIndex + 13] = (integer >> 13) & 1;
	(*m_data)[m_writeIndex + 14] = (integer >> 14) & 1;
	(*m_data)[m_writeIndex + 15] = (integer >> 15) & 1;

	m_writeIndex += 16;
}
