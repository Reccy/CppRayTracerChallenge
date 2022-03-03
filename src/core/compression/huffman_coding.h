#ifndef _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_HUFFMAN_CODING
#define _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_HUFFMAN_CODING

#include <vector>
#include <memory>
#include <map>

namespace CppRayTracerChallenge::Core::Compression
{
	enum HuffmanNodeType
	{
		INTERNAL,
		LEAF
	};
	
	struct HuffmanNode
	{
		unsigned int frequency = 0;
		unsigned char data = '\0';
		HuffmanNodeType type = LEAF;
		std::weak_ptr<HuffmanNode> parent;
		std::weak_ptr<HuffmanNode> left;
		std::weak_ptr<HuffmanNode> right;
	};

	bool operator<(const HuffmanNode& lhs, const HuffmanNode& rhs);

	using CodeMap = std::vector<std::tuple<unsigned char, std::vector<int>>>;

	class HuffmanCoding
	{
	public:
		/// <summary>
		/// Generates the Huffman tree
		/// </summary>
		/// <param name="bytes">Vector of bytes to generate the tree</param>
		HuffmanCoding(std::vector<unsigned char> bytes);

		/// <summary>
		/// Retrieves the huffman code for a byte
		/// </summary>
		std::vector<int> lookupHuffman(unsigned char originalByte);
	private:
		CodeMap m_codes;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_COMPRESSION_HUFFMAN_CODING