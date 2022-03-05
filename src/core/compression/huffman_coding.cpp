#include "huffman_coding.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <map>
#include <stack>

using namespace CppRayTracerChallenge::Core::Compression;

class HuffmanNodeQueueCompare {
public:
	bool operator() (std::shared_ptr<HuffmanNode> lhs, std::shared_ptr<HuffmanNode> rhs)
	{
		// Compare frequency first. If greater or equal, sort by alphabetical order / ASCII table order
		// Also fulfills strict weak ordering condition
		return (lhs->frequency > rhs->frequency);
	}
};

using HuffmanNodeQueue = std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, HuffmanNodeQueueCompare>;

bool CppRayTracerChallenge::Core::Compression::operator<(const HuffmanNode& lhs, const HuffmanNode& rhs)
{
	return lhs.frequency > rhs.frequency;
}

std::map<unsigned char, unsigned int> buildFrequencyMap(std::vector<unsigned char> &bytes)
{
	std::map<unsigned char, unsigned int> frequencyMap;

	for (size_t i = 0; i < bytes.size(); ++i)
	{
		frequencyMap[bytes[i]]++;
	}

	return frequencyMap;
}

HuffmanNodeQueue buildNodeQueue(std::map<unsigned char, unsigned int> frequencyMap)
{
	HuffmanNodeQueue nodes;

	for (auto const& [data, frequency] : frequencyMap)
	{
		auto newNode = std::make_shared<HuffmanNode>();
		newNode->frequency = frequency;
		newNode->data = data;
		newNode->type = LEAF;

		nodes.push(newNode);
	}

	return nodes;
}

CodeMap generateCodeMap(std::vector<std::weak_ptr<HuffmanNode>> const & leafNodes)
{
	CodeMap codeMap;

	for (auto i = 0; i < leafNodes.size(); ++i)
	{
		std::shared_ptr<HuffmanNode> checkingNode(leafNodes[i]);
		std::shared_ptr<HuffmanNode> node = checkingNode;
		std::shared_ptr<HuffmanNode> prevNode = nullptr;
		std::stack<unsigned char> codeStack;
		std::vector<int> code;

		while (!node->parent.expired())
		{
			prevNode = node;
			node = std::shared_ptr<HuffmanNode>(node->parent);

			if (std::shared_ptr<HuffmanNode>(node->left) == prevNode)
			{
				codeStack.push(0);
			}
			else
			{
				codeStack.push(1);
			}
		}

		while (!codeStack.empty())
		{
			code.push_back(codeStack.top());
			codeStack.pop();
		}

		codeMap.push_back({ checkingNode->data, code });
	}

	return codeMap;
}

using BitlengthTuple = std::tuple<unsigned char, unsigned int>;
using BitlengthMap = std::vector<BitlengthTuple>;

BitlengthMap buildBitlengthMap(CodeMap codemap)
{
	BitlengthMap bitlengthMap;

	for (auto it = codemap.begin(); it != codemap.end(); ++it)
	{
		auto character = std::get<0>(*it);
		auto huffmanCode = std::get<1>(*it);

		bitlengthMap.push_back({ character, (unsigned int) huffmanCode.size() });
	}

	struct {
		bool operator()(BitlengthTuple a, BitlengthTuple b) const
		{
			auto aCodeLength = std::get<1>(a);
			auto bCodeLength = std::get<1>(b);

			if (aCodeLength < bCodeLength)
				return true;

			if (aCodeLength == bCodeLength)
			{
				auto aChar = std::get<0>(a);
				auto bChar = std::get<1>(b);

				return (aChar < bChar);
			}

			return false;
		}
	} Custom;

	std::sort(bitlengthMap.begin(), bitlengthMap.end(), Custom);

	return bitlengthMap;
}

CodeMap buildCanonicalCodemap(BitlengthMap lengthmap)
{
	return CodeMap();
}

HuffmanCoding::HuffmanCoding(std::vector<unsigned char> bytes)
{
	std::map<unsigned char, unsigned int> frequencyMap = buildFrequencyMap(bytes);
	HuffmanNodeQueue nodeQueue = buildNodeQueue(frequencyMap);
	std::vector<std::shared_ptr<HuffmanNode>> nodes;
	std::vector<std::weak_ptr<HuffmanNode>> leafNodes;

	while (nodeQueue.size() > 1)
	{
		// Pop top two nodes and copy to final vector
		nodes.push_back(nodeQueue.top());
		const auto nodeA = nodes.back();
		nodeQueue.pop();

		nodes.push_back(nodeQueue.top());
		const auto nodeB = nodes.back();
		nodeQueue.pop();

		// Set parent node
		auto parent = std::make_shared<HuffmanNode>();
		parent->frequency = nodeA->frequency + nodeB->frequency;
		parent->type = INTERNAL;
		parent->left = nodeA;
		parent->right = nodeB;

		nodeA->parent = parent;
		nodeB->parent = parent;

		if (nodeA->type == LEAF)
			leafNodes.push_back(nodeA);

		if (nodeB->type == LEAF)
			leafNodes.push_back(nodeB);

		nodeQueue.push(parent);
	}

	std::shared_ptr<HuffmanNode> root = nodeQueue.top();
	nodes.push_back(root);

	m_codes = generateCodeMap(leafNodes);

	BitlengthMap bitlengthMap = buildBitlengthMap(m_codes);

	//m_codes = buildCanonicalCodemap(bitlengthMap);
}

std::vector<int> HuffmanCoding::lookupHuffman(unsigned char originalByte)
{
	for (auto i = 0; i < m_codes.size(); ++i)
	{
		auto const& code = m_codes[i];

		if (std::get<0>(code) == originalByte)
		{
			return std::get<1>(code);
		}
	}

	// Error: No byte found
	return std::vector<int>();
}
