#include "portable_network_graphics_serializer.h"
#include "../graphics/color.h"
#include "../encryption/crc.h"

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
	append(this->m_buffer, buildIDATChunk());
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

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildImageData()
{
	std::vector<unsigned char> imageData
	{
		0,	// zlib compression method/flags code
		0	// Additional flags/check bits
	};

	append(imageData, 0);

	std::vector<Graphics::Color> imageBuffer = this->m_image.toBuffer();

	for (int i = 0; i < imageBuffer.size(); ++i)
	{

	}

	//append(imageData, buildZLIPCheck(imageData));

	return imageData;
}

std::vector<unsigned char> PortableNetworkGraphicsSerializer::buildCRC(std::vector<unsigned char> chunk, int chunkDataLength)
{
	int chunkLengthOffset = 4;
	int chunkTypeOffset = 4;

	std::vector<unsigned char> track = std::vector(chunk.begin() + chunkLengthOffset, chunk.begin() + chunkLengthOffset + chunkTypeOffset + chunkDataLength);

	return Encryption::CRC::run(track);
}

void PortableNetworkGraphicsSerializer::append(std::vector<unsigned char>& target, std::vector<unsigned char> source)
{
	target.insert(target.end(), source.begin(), source.end());
}

void PortableNetworkGraphicsSerializer::append(std::vector<unsigned char>& target, unsigned char data)
{
	target.push_back(data);
}
