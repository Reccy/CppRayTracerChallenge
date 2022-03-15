#include "deflate.h"
#include "huffman_coding.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace CppRayTracerChallenge::Core::Compression;

DeflateBlock::DeflateBlock(std::vector<unsigned char> data, bool isFinal)
	: m_writeIndex(0), m_data(std::make_unique<DeflateBitset>())
{
	if (data.size() > MAX_BYTES)
	{
		std::stringstream ss;
		ss << "Cannot create uncompressed DEFLATE block with more than " << MAX_BYTES << " bytes!";
		std::string error = ss.str();
		std::cerr << error << std::endl;
		throw std::logic_error(error);
	}

	unsigned short lengthBytes = static_cast<unsigned short>(data.size());

	// Set BFINAL
	writeBit(isFinal);

	// Set BTYPE to 0 (Uncompressed)
	writeBit(0);
	writeBit(0);

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

void DeflateBlock::writeInt(unsigned int integer)
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

	(*m_data)[m_writeIndex + 16] = (integer >> 16) & 1;
	(*m_data)[m_writeIndex + 17] = (integer >> 17) & 1;
	(*m_data)[m_writeIndex + 18] = (integer >> 18) & 1;
	(*m_data)[m_writeIndex + 19] = (integer >> 19) & 1;
	(*m_data)[m_writeIndex + 20] = (integer >> 20) & 1;
	(*m_data)[m_writeIndex + 21] = (integer >> 21) & 1;
	(*m_data)[m_writeIndex + 22] = (integer >> 22) & 1;
	(*m_data)[m_writeIndex + 23] = (integer >> 23) & 1;

	(*m_data)[m_writeIndex + 24] = (integer >> 24) & 1;
	(*m_data)[m_writeIndex + 25] = (integer >> 25) & 1;
	(*m_data)[m_writeIndex + 26] = (integer >> 26) & 1;
	(*m_data)[m_writeIndex + 27] = (integer >> 27) & 1;
	(*m_data)[m_writeIndex + 28] = (integer >> 28) & 1;
	(*m_data)[m_writeIndex + 29] = (integer >> 29) & 1;
	(*m_data)[m_writeIndex + 30] = (integer >> 30) & 1;
	(*m_data)[m_writeIndex + 31] = (integer >> 31) & 1;

	m_writeIndex += 32;
}
