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

TEST(CppRayTracerChallange_Core_Compression_Deflate, deflate_block_sets_header_bfinal_true)
{
	auto block = testDeflateBlockFinal();

	auto data = block.data();

	EXPECT_EQ(data[0], 1);
}

TEST(CppRayTracerChallange_Core_Compression_Deflate, deflate_block_sets_header_bfinal_false)
{
	auto block = testDeflateBlock();

	auto data = block.data();

	EXPECT_EQ(data[0], 0);
}

TEST(CppRayTracerChallange_Core_Compression_Deflate, deflate_block_sets_header_btype_to_dynamic_huffman)
{
	auto block = testDeflateBlock();

	auto data = block.data();

	EXPECT_EQ(data[1], 1);
	EXPECT_EQ(data[2], 0);
}
