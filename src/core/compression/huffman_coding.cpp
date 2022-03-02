#include "huffman_coding.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <map>

using namespace CppRayTracerChallenge::Core::Compression;

class HuffmanNodeQueueCompare {
public:
	bool operator() (std::shared_ptr<HuffmanNode> lhs, std::shared_ptr<HuffmanNode> rhs)
	{
		return lhs->frequency > rhs->frequency;
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

HuffmanCoding::HuffmanCoding(std::vector<unsigned char> bytes)
{
	std::map<unsigned char, unsigned int> frequencyMap = buildFrequencyMap(bytes);
	HuffmanNodeQueue nodes = buildNodeQueue(frequencyMap);

	while (nodes.size() > 1)
	{
		// Pop top two nodes and copy to final vector
		m_nodes.push_back(nodes.top());
		const auto nodeA = m_nodes.back();
		nodes.pop();

		m_nodes.push_back(nodes.top());
		const auto nodeB = m_nodes.back();
		nodes.pop();

		// Set parent node
		auto parent = std::make_shared<HuffmanNode>();
		parent->frequency = nodeA->frequency + nodeB->frequency;
		parent->type = INTERNAL;
		parent->left = nodeA;
		parent->right = nodeB;

		nodes.push(parent);
	}

	m_root = nodes.top();
	m_nodes.push_back(m_root);
}

std::vector<int> HuffmanCoding::lookupHuffman(unsigned char originalByte)
{
	return std::vector<int>(originalByte);
}
