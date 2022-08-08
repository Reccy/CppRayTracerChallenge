#pragma once
#include "base_image_serializer.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

namespace CppRayTracerChallenge::Core::Serializer
{
	/// <summary>
	/// A PortableNetworkGraphics serializer responsible for serializing an image to a buffer and vice versa
	/// </summary>
	class PortableNetworkGraphicsSerializer : public BaseImageSerializer
	{
	public:
		void serialize(Graphics::Image image) override;
		std::string fileExtension() const override;
	private:
		std::vector<unsigned char> buildSignature();
		std::vector<unsigned char> buildIHDRChunk();
		std::vector<unsigned char> buildIDATChunk();
		std::vector<unsigned char> buildIENDChunk();
		std::vector<unsigned char> buildImageData();
		std::vector<unsigned char> buildCRC(std::vector<unsigned char> chunk, int chunkLength);
		void append(std::vector<unsigned char> &target, std::vector<unsigned char> source);
		void append(std::vector<unsigned char>& target, unsigned char value);
	};
}
