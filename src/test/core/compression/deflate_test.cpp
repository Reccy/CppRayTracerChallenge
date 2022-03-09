#include <gtest/gtest.h>
#include "compression/deflate.h"

using namespace CppRayTracerChallenge::Core::Compression;

DeflateBlock testDeflateBlock()
{
	return DeflateBlock({ 'h', 'e', 'l', 'l', 'o' }, false);
}

DeflateBlock testDeflateBlockFinal()
{
	return DeflateBlock({ 'h', 'e', 'l', 'l', 'o' }, true);
}

DeflateBlock testUncompressedDeflateBlock()
{
	return DeflateBlock({ 'h', 'e', 'l', 'l', 'o' }, true, false);
}

TEST(CppRayTracerChallange_Core_Compression_Deflate, deflate_block_sets_header_bfinal_true)
{
	auto block = testDeflateBlockFinal();

	auto const& data = block.data();

	EXPECT_EQ(data[0], 1);
}

TEST(CppRayTracerChallange_Core_Compression_Deflate, deflate_block_sets_header_bfinal_false)
{
	auto block = testDeflateBlock();

	auto const & data = block.data();

	EXPECT_EQ(data[0], 0);
}

TEST(CppRayTracerChallange_Core_Compression_Deflate, deflate_block_sets_header_btype_to_dynamic_huffman)
{
	auto block = testDeflateBlock();

	auto const& data = block.data();

	EXPECT_EQ(data[1], 0);
	EXPECT_EQ(data[2], 1);
}

TEST(CppRayTracerChallenge_Core_Compression_Deflate, uncompressed_writes_padding)
{
	auto block = testUncompressedDeflateBlock();

	auto const& data = block.data();

	EXPECT_EQ(data[3], 0);
	EXPECT_EQ(data[4], 0);
	EXPECT_EQ(data[5], 0);
	EXPECT_EQ(data[6], 0);
	EXPECT_EQ(data[7], 0);
}

TEST(CppRayTracerChallenge_Core_Compression_Deflate, uncompressed_writes_length_and_complement)
{
	auto block = testUncompressedDeflateBlock();

	auto const& data = block.data();

	// Length (16 bit value)
	int offset = 8;

	EXPECT_EQ(data[0 + offset], 1);
	EXPECT_EQ(data[1 + offset], 0);
	EXPECT_EQ(data[2 + offset], 1);
	EXPECT_EQ(data[3 + offset], 0);
	EXPECT_EQ(data[4 + offset], 0);
	EXPECT_EQ(data[5 + offset], 0);
	EXPECT_EQ(data[6 + offset], 0);
	EXPECT_EQ(data[7 + offset], 0);
	EXPECT_EQ(data[8 + offset], 0);
	EXPECT_EQ(data[9 + offset], 0);
	EXPECT_EQ(data[10 + offset], 0);
	EXPECT_EQ(data[11 + offset], 0);
	EXPECT_EQ(data[12 + offset], 0);
	EXPECT_EQ(data[13 + offset], 0);
	EXPECT_EQ(data[14 + offset], 0);
	EXPECT_EQ(data[15 + offset], 0);

	// Complement (16 bit value)
	offset = offset + 16;

	EXPECT_EQ(data[0 + offset], 0);
	EXPECT_EQ(data[1 + offset], 1);
	EXPECT_EQ(data[2 + offset], 0);
	EXPECT_EQ(data[3 + offset], 1);
	EXPECT_EQ(data[4 + offset], 1);
	EXPECT_EQ(data[5 + offset], 1);
	EXPECT_EQ(data[6 + offset], 1);
	EXPECT_EQ(data[7 + offset], 1);
	EXPECT_EQ(data[8 + offset], 1);
	EXPECT_EQ(data[9 + offset], 1);
	EXPECT_EQ(data[10 + offset], 1);
	EXPECT_EQ(data[11 + offset], 1);
	EXPECT_EQ(data[12 + offset], 1);
	EXPECT_EQ(data[13 + offset], 1);
	EXPECT_EQ(data[14 + offset], 1);
	EXPECT_EQ(data[15 + offset], 1);
}

unsigned char readByte(DeflateBlock::DeflateBitset const & bitset, int offset)
{
	std::bitset<8> bits;

	bits[0] = bitset[0 + offset];
	bits[1] = bitset[1 + offset];
	bits[2] = bitset[2 + offset];
	bits[3] = bitset[3 + offset];
	bits[4] = bitset[4 + offset];
	bits[5] = bitset[5 + offset];
	bits[6] = bitset[6 + offset];
	bits[7] = bitset[7 + offset];

	return static_cast<unsigned char>(bits.to_ulong());
}

TEST(CppRayTracerChallenge_Core_Compression_Deflate, uncompressed_writes_bitstream)
{
	auto block = testUncompressedDeflateBlock();

	auto const& data = block.data();

	int offset = 40;

	EXPECT_EQ(readByte(data, offset + 0), 'h');
	EXPECT_EQ(readByte(data, offset + 8), 'e');
	EXPECT_EQ(readByte(data, offset + 16), 'l');
	EXPECT_EQ(readByte(data, offset + 24), 'l');
	EXPECT_EQ(readByte(data, offset + 32), 'o');
}
