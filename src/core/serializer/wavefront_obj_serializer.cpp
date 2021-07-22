#include "wavefront_obj_serializer.h"
#include <cstdlib>
#include <cerrno>
#include <string>

using namespace CppRayTracerChallenge::Core::Serializer;
using namespace CppRayTracerChallenge::Core;

void WavefrontOBJSerializer::serialize()
{}

void WavefrontOBJSerializer::deserialize(std::vector<char> input)
{
	m_buffer = input;
	m_vertices = std::vector<Math::Point>();
	m_group = Renderer::Group();
	m_ignoredLines = 0;

	std::vector<std::string> lines;
	int lineBegin = 0;

	// Parse data into lines
	for (int i = 0; i < input.size(); ++i)
	{
		char byte = input[i];

		// If we are at a newline
		if (byte == '\n')
		{
			lines.push_back({ input.begin() + lineBegin, input.begin() + i });
			lineBegin = i + 1;
			continue;
		}

		// If we are at the last char in the buffer
		if (i == input.size() - 1)
		{
			lines.push_back({ input.begin() + lineBegin, input.begin() + i + 1 });
			continue;
		}
	}

	// Parse each line
	for (int i = 0; i < lines.size(); ++i)
	{
		std::string& line = lines[i];

		// If line is completely empty, do nothing
		if (line.size() == 0)
		{
			m_ignoredLines++;
			continue;
		}

		// Parse vertex instruction
		if (line[0] == 'v')
		{
			parseVertex(line);
			continue;
		}

		// Parse face instruction
		if (line[0] == 'f')
		{
			parseFace(line);
			continue;
		}

		// Unknown instruction, ignore
		m_ignoredLines++;
	}
}

void WavefrontOBJSerializer::parseVertex(std::string line)
{
	double values[3] = { -123, -312, -231 };
	int valueIndex = 0;

	int lastWhitespace = 0;
	int tokenStart = 0;

	for (int j = 1; j < line.size(); ++j)
	{
		char byte = line[j];

		// More than 3 vertices in instruction which is not valid
		if (valueIndex > 2)
		{
			m_ignoredLines++;
			return;
		}

		// If current byte is a whitespace or we are at end of line
		if (std::isspace(byte) > 0 || j == line.size() - 1)
		{
			if (j == line.size() - 1)
			{
				tokenStart = j;
			}

			// tokenStart is greater than lastWhitespace only if we are currently parsing a token
			if (tokenStart > lastWhitespace)
			{
				std::string token = std::string(line.begin() + tokenStart, line.begin() + j + 1);

				try
				{
					values[valueIndex] = std::stod(token, nullptr);
				}
				catch (const std::invalid_argument& ia)
				{
					std::cerr << "Invalid argument when parsing OBJ file: " << ia.what() << "\n";
					m_ignoredLines++;
					return;
				}

				++valueIndex;
			}

			lastWhitespace = j;
			continue;
		}

		if (tokenStart < lastWhitespace)
		{
			tokenStart = j;
		}
	}

	m_vertices.push_back(Math::Point(values[0], values[1], values[2]));
}

void WavefrontOBJSerializer::parseFace(std::string line)
{
	int indices[3] = { -1, -1, -1 };
	int valueIndex = 0;

	int lastWhitespace = 0;
	int tokenStart = 0;

	for (int j = 1; j < line.size(); ++j)
	{
		char byte = line[j];

		// More than 3 indices in instruction which is not valid
		if (valueIndex > 2)
		{
			m_ignoredLines++;
			return;
		}

		// If current byte is a whitespace or we are at end of line
		if (std::isspace(byte) > 0 || j == line.size() - 1)
		{
			if (j == line.size() - 1)
			{
				tokenStart = j;
			}

			// tokenStart is greater than lastWhitespace only if we are currently parsing a token
			if (tokenStart > lastWhitespace)
			{
				std::string token = std::string(line.begin() + tokenStart, line.begin() + j + 1);
				indices[valueIndex] = atoi(token.c_str());
				++valueIndex;
			}

			lastWhitespace = j;
			continue;
		}

		if (tokenStart < lastWhitespace)
		{
			tokenStart = j;
		}
	}

	if (indices[0] == -1 || indices[1] == -1 || indices[2] == -1)
	{
		m_ignoredLines++;
		return;
	}

	if (indices[0] > m_vertices.size() || indices[1] > m_vertices.size() || indices[2] > m_vertices.size())
	{
		m_ignoredLines++;
		return;
	}
	
	Math::Point v1 = m_vertices.at(indices[0] - 1);
	Math::Point v2 = m_vertices.at(indices[1] - 1);
	Math::Point v3 = m_vertices.at(indices[2] - 1);

	auto triangle = std::make_shared<Math::Triangle>(v1, v2, v3);
	auto shape = std::make_shared<Renderer::Shape>(triangle);

	m_group.addChild(shape);
}

std::vector<char> WavefrontOBJSerializer::buffer() const
{
	return m_buffer;
}

Renderer::Group WavefrontOBJSerializer::defaultGroup() const
{
	return m_group;
}

std::vector<Math::Point> WavefrontOBJSerializer::vertices() const
{
	return m_vertices;
}

int WavefrontOBJSerializer::ignoredLines() const
{
	return m_ignoredLines;
}
