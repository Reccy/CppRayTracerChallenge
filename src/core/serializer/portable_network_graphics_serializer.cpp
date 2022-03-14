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

	for (unsigned int i = 0; i < imageBuffer.size(); ++i)
	{
		auto color = imageBuffer[i];

		// Insert filter byte at start of scanline
		if (i % m_image.width() == 0)
		{
			//pixelData.push_back('S');
			pixelData.push_back(0);
		}

		//pixelData.push_back('R');
		//pixelData.push_back('G');
		//pixelData.push_back('B');

		pixelData.push_back(convertColorValue(color.red()));
		pixelData.push_back(convertColorValue(color.green()));
		pixelData.push_back(convertColorValue(color.blue()));
	}

	unsigned int deflateBlockCount = static_cast<unsigned int>(std::ceil((double)pixelData.size() / (double)Compression::DeflateBlock::MAX_BYTES));

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

	auto pixelDataSize = pixelData.size();
	auto maxBytes = Compression::DeflateBlock::MAX_BYTES;

	std::cout << "Deflate: Compressing with " << (deflateBlockCount + 1) << " chunks\n";

	for (unsigned int deflateIndex = 0; deflateIndex < deflateBlockCount; ++deflateIndex)
	{
		auto startIdx = maxBytes * deflateIndex;
		auto endIdx = startIdx + maxBytes;

		bool isFinal = deflateIndex == deflateBlockCount - 1;

		if (isFinal)
			endIdx = static_cast<unsigned int>(pixelDataSize);

		auto startIt = pixelData.begin() + startIdx;
		auto endIt = pixelData.begin() + endIdx;

		std::vector<unsigned char> pixelDataChunk(endIdx - startIdx);
		std::copy(startIt, endIt, pixelDataChunk.begin());

		auto deflate = Compression::DeflateBlock(pixelDataChunk, isFinal, false);

		auto deflateSize = deflate.size();
		auto const & bitset = deflate.data();

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
