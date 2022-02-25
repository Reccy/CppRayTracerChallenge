#include "portable_network_graphics_serializer.h"

using namespace CppRayTracerChallenge::Core::Serializer;

//
// CRC Algorithm from https://www.w3.org/TR/PNG/#D-CRCAppendix
//

/* Table of CRCs of all 8-bit messages. */
unsigned long crc_table[256];

/* Flag: has the table been computed? Initially false. */
int crc_table_computed = 0;

/* Make the table for a fast CRC. */
void make_crc_table(void)
{
	unsigned long c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = (unsigned long)n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = 1;
}


/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below). */

unsigned long update_crc(unsigned long crc, unsigned char* buf,
	int len)
{
	unsigned long c = crc;
	int n;

	if (!crc_table_computed)
		make_crc_table();
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}

/* Return the CRC of the bytes buf[0..len-1]. */
unsigned long crc(unsigned char* buf, int len)
{
	return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

//
// End of CRC algorithm
//

std::vector<unsigned char> intToBytes(int value)
{
	std::vector<unsigned char> result;

	result.push_back((unsigned char)(value >> 24) & 0xFF);
	result.push_back((unsigned char)(value >> 16) & 0xFF);
	result.push_back((unsigned char)(value >> 8) & 0xFF);
	result.push_back((unsigned char)value & 0xFF);

	return result;
}

std::vector<unsigned char> longToBytes(unsigned long value)
{
	std::vector<unsigned char> result;

	result.push_back((unsigned char)(value >> 24) & 0xFF);
	result.push_back((unsigned char)(value >> 16) & 0xFF);
	result.push_back((unsigned char)(value >> 8) & 0xFF);
	result.push_back((unsigned char)value & 0xFF);

	return result;
}

void PortableNetworkGraphicsSerializer::serialize(Graphics::Image image)
{
	this->m_image = image;

	this->m_buffer.clear();
	append(this->m_buffer, buildSignature());
	append(this->m_buffer, buildIHDRChunk());
	append(this->m_buffer, buildIENDChunk());
}

void PortableNetworkGraphicsSerializer::deserialize(std::vector<unsigned char> buffer)
{
	this->m_buffer = buffer;

	// TODO: Implementation
}

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildSignature()
{
	std::vector<unsigned char> signature { 137, 80, 78, 71, 13, 10, 26, 10 };
	return signature;
}

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildIHDRChunk()
{
	std::vector<unsigned char> chunkLength;
	int chunkLengthInt = 13;
	append(chunkLength, intToBytes(chunkLengthInt));

	std::vector<unsigned char> chunkType { 73, 72, 68, 82 };

	std::vector<unsigned char> chunk;

	// Chunk Header
	append(chunk, chunkLength);
	append(chunk, chunkType);

	// Image width and height
	append(chunk, intToBytes(this->m_image.width()));
	append(chunk, intToBytes(this->m_image.height()));

	// Image other details
	append(chunk, 8);	// Bit Depth
	append(chunk, 2);	// Color Mode (True Color)
	append(chunk, 0);	// Compression Method
	append(chunk, 0);	// Filter
	append(chunk, 0);	// Interlace

	// CRC
	append(chunk, buildCRC(chunk, chunkLengthInt));

	return chunk;
}

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildIENDChunk()
{
	std::vector<unsigned char> chunkLength;
	int chunkLengthInt = 0;
	append(chunkLength, intToBytes(chunkLengthInt));

	std::vector<unsigned char> chunkType{ 73, 69, 78, 68 };

	std::vector<unsigned char> chunk;

	// Chunk Header
	append(chunk, chunkLength);
	append(chunk, chunkType);

	// CRC
	append(chunk, buildCRC(chunk, chunkLengthInt));

	return chunk;
}

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildCRC(std::vector<unsigned char> chunk, int chunkDataLength)
{
	int chunkLengthOffset = 4;
	int chunkTypeOffset = 4;

	std::vector<unsigned char> track = std::vector(chunk.begin() + chunkLengthOffset, chunk.begin() + chunkLengthOffset + chunkTypeOffset + chunkDataLength);

	unsigned long resultLong = crc(track.data(), (int)track.size());

	return longToBytes(resultLong);
}

void PortableNetworkGraphicsSerializer::append(std::vector<unsigned char>& target, std::vector<unsigned char> source)
{
	target.insert(target.end(), source.begin(), source.end());
}

void PortableNetworkGraphicsSerializer::append(std::vector<unsigned char>& target, unsigned char data)
{
	target.push_back(data);
}