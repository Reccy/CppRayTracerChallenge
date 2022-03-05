#include "gtest/gtest.h"
#include "compression/huffman_coding.h"

#include <vector>

using namespace CppRayTracerChallenge::Core;

std::vector<unsigned char> createFrequency(unsigned char character, unsigned int frequency)
{
	std::vector<unsigned char> result;

	for (unsigned int i = 0; i < frequency; ++i)
	{
		result.push_back(character);
	}

	return result;
}

TEST(CppRayTracerChallenge_Core_Compression_HuffmanCoding, generates_tree)
{
	std::vector<unsigned char> input;

	auto a = createFrequency('a', 10);
	auto b = createFrequency('b', 1);
	auto c = createFrequency('c', 15);
	auto d = createFrequency('d', 7);

	input.insert(input.end(), a.begin(), a.end());
	input.insert(input.end(), b.begin(), b.end());
	input.insert(input.end(), c.begin(), c.end());
	input.insert(input.end(), d.begin(), d.end());

	Compression::HuffmanCoding huffman(input);

	auto aResult = std::vector<int>{ 1, 1 };
	auto bResult = std::vector<int>{ 1, 0, 0 };
	auto cResult = std::vector<int>{ 0 };
	auto dResult = std::vector<int>{ 1, 0, 1 };

	EXPECT_EQ(huffman.lookupHuffman('a'), aResult);
	EXPECT_EQ(huffman.lookupHuffman('b'), bResult);
	EXPECT_EQ(huffman.lookupHuffman('c'), cResult);
	EXPECT_EQ(huffman.lookupHuffman('d'), dResult);
}
