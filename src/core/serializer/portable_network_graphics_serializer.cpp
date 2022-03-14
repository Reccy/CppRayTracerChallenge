#include "portable_network_graphics_serializer.h"
#include "../graphics/color.h"
#include "../encryption/crc.h"
#include "../encryption/adler32.h"
#include "../compression/deflate.h"

using namespace CppRayTracerChallenge::Core::Serializer;

std::vector<unsigned char> intToBytes(int value)
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
	append(this->m_buffer, buildIDATChunk());
	append(this->m_buffer, buildIENDChunk());
}

std::string PortableNetworkGraphicsSerializer::fileExtension() const
{
	return std::string("png");
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

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildIDATChunk()
{
	std::vector<unsigned char> imageData = buildImageData();

	std::vector<unsigned char> chunkLength;
	int chunkLengthInt = (int)imageData.size();
	append(chunkLength, intToBytes(chunkLengthInt));

	std::vector<unsigned char> chunkType{ 73, 68, 65, 84 };

	std::vector<unsigned char> chunk;

	// Chunk Header
	append(chunk, chunkLength);
	append(chunk, chunkType);

	// Image Data
	append(chunk, imageData);

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

unsigned char setWithFCHECK(unsigned char cm, unsigned char cinfo)
{
	// Data size is assumed to be 16 bits

	// TODO: Figure out algebraic formula for determining the FCHECK, without doing a for loop

	std::bitset<8> cmBits = cm;
	std::bitset<8> cinfoBits = cinfo;

	for (int i = 0; i < 16; ++i)
	{
		std::bitset<16> checkBits;
		std::bitset<4> indexBits = i;

		// Set the iterated number
		checkBits[0] = indexBits[0];
		checkBits[1] = indexBits[1];
		checkBits[2] = indexBits[2];
		checkBits[3] = indexBits[3];

		// Set the cinfo static data
		checkBits[4] = cinfoBits[0];
		checkBits[5] = cinfoBits[1];
		checkBits[6] = cinfoBits[2];
		checkBits[7] = cinfoBits[3];

		// Set the cm
		checkBits[8] = cmBits[0];
		checkBits[9] = cmBits[1];
		checkBits[10] = cmBits[2];
		checkBits[11] = cmBits[3];
		checkBits[12] = cmBits[4];
		checkBits[13] = cmBits[5];
		checkBits[14] = cmBits[6];
		checkBits[15] = cmBits[7];

		auto check = static_cast<unsigned int>(checkBits.to_ulong());

		if (check % 31 == 0 || check == 0)
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

	throw std::logic_error("Could not compute a valid FCHECK when encoding PNG file");
}

bool inline getBit(CppRayTracerChallenge::Core::Compression::DeflateBlock::DeflateBitset const& bitset, unsigned int index)
{
	if (index >= bitset.size())
		return false;

	return bitset.test(index);
}

unsigned char convertColorValue(float colorValue) // TODO: Merge with PPM to base class or helper class
{
	float scaledColor = colorValue * 255;
	scaledColor = std::ceil(scaledColor);
	scaledColor = std::clamp(scaledColor, 0.0f, 255.0f);

	return static_cast<unsigned char>(scaledColor);
}

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildImageData()
{
	std::vector<unsigned char> pixelData;

	auto const& imageBuffer = m_image.toBuffer();

	for (auto it = imageBuffer.begin(); it != imageBuffer.end(); ++it)
	{
		auto color = *it;

		pixelData.push_back(convertColorValue(color.red()));
		pixelData.push_back(convertColorValue(color.green()));
		pixelData.push_back(convertColorValue(color.blue()));
	}

	auto deflate = Compression::DeflateBlock(pixelData, true, false);

	auto deflateSize = deflate.size();
	auto const & bitset = deflate.data();

	unsigned int numBytes = static_cast<int>(ceil(deflateSize / 8));

	// zlib compression method/ flags code (CMF)
	// CM (Compression Method) is "deflate" method for PNGs - 1000 (8)
	// CINFO (Compression Info) is "32k window size for LZ77" - 0111 (7)
	// Therefore the zlib compression method / flags code is 10000111 (135)
	//
	// Additional flags / check bits (FLG)
	// FCHECK
	// FDICT - If a dictionary is present after the FLG bit - 0 (0)
	// FLEVEL - Uses default algorithm - 010 (2)
	std::vector<unsigned char> result;

	unsigned char cm = 0b01111000;
	unsigned char cinfo = 0b11000000;

	result.push_back(cm); // zlib compression method/flags code
	result.push_back(setWithFCHECK(cm, cinfo));

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

	// Split adler32 into 4 bytes (MSB)
	auto adler32 = Encryption::Adler32::run(pixelData);
	append(result, ((unsigned char*)(&adler32))[3]);
	append(result, ((unsigned char*)(&adler32))[2]);
	append(result, ((unsigned char*)(&adler32))[1]);
	append(result, ((unsigned char*)(&adler32))[0]);

	return result;
}

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildCRC(std::vector<unsigned char> chunk, int chunkDataLength)
{
	int chunkLengthOffset = 4;
	int chunkTypeOffset = 4;

	std::vector<unsigned char> track = std::vector(chunk.begin() + chunkLengthOffset, chunk.begin() + chunkLengthOffset + chunkTypeOffset + chunkDataLength);

	return Encryption::CRC::run(track);
}

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildZLIBCheck(std::vector<unsigned char> data)
{
	return std::vector<unsigned char>();
}

void PortableNetworkGraphicsSerializer::append(std::vector<unsigned char>& target, std::vector<unsigned char> source)
{
	target.insert(target.end(), source.begin(), source.end());
}

void PortableNetworkGraphicsSerializer::append(std::vector<unsigned char>& target, unsigned char data)
{
	target.push_back(data);
}
