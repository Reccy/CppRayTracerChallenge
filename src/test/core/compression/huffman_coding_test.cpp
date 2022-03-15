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

bool vectorContains(std::vector<unsigned char> const& vector, unsigned char value)
{
	return (std::find(vector.begin(), vector.end(), value) != vector.end());
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

	auto aResult = std::vector<int>{ 1, 0 };
	auto bResult = std::vector<int>{ 1, 1, 0 };
	auto cResult = std::vector<int>{ 0 };
	auto dResult = std::vector<int>{ 1, 1, 1 };

	EXPECT_EQ(huffman.lookupHuffman('a'), aResult);
	EXPECT_EQ(huffman.lookupHuffman('b'), bResult);
	EXPECT_EQ(huffman.lookupHuffman('c'), cResult);
	EXPECT_EQ(huffman.lookupHuffman('d'), dResult);
}

TEST(CppRayTracerChallenge_Core_Compression_HuffmanCoding, generates_list_of_available_characters)
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

	auto result = huffman.encodedCharacters();

	EXPECT_EQ(result.size(), 4);
	EXPECT_TRUE(vectorContains(result, 'a'));
	EXPECT_TRUE(vectorContains(result, 'b'));
	EXPECT_TRUE(vectorContains(result, 'c'));
	EXPECT_TRUE(vectorContains(result, 'd'));
}
