#include "gtest/gtest.h"
#include "compression/huffman_coding.h"

#include <vector>

using namespace CppRayTracerChallenge::Core;

TEST(CppRayTracerChallenge_Core_Compression_HuffmanCoding, generates_tree)
{
	std::vector<unsigned char> input = {
		'h', 'e', 'l', 'l', 'o'
		//'5', '4', '3', '4', '3', '5', '1', '2', '3', '2', '4', '5', '4', '5', '5'
	};

	Compression::HuffmanCoding huffman(input);

	auto hResult = std::vector<int>{ 1, 1, 0 };
	auto eResult = std::vector<int>{ 1, 1, 1 };
	auto lResult = std::vector<int>{ 0, 0 };
	auto oResult = std::vector<int>{ 1, 0 };

	EXPECT_EQ(huffman.lookupHuffman('h'), hResult);
	EXPECT_EQ(huffman.lookupHuffman('e'), eResult);
	EXPECT_EQ(huffman.lookupHuffman('l'), lResult);
	EXPECT_EQ(huffman.lookupHuffman('o'), oResult);
}
