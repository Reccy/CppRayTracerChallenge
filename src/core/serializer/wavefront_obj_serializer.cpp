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
	m_defaultGroup = Renderer::Group();
	m_groups = std::map<std::string, std::shared_ptr<Renderer::Group>>();
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

		if (line[0] == 'f')
		{
			parseFace(line);
			continue;
		}

		if (line[0] == 'g')
		{
			parseGroup(line);
			continue;
		}

		if (line[0] == 'v' && line[1] == 'n')
		{
			parseNormal(line);
			continue;
		}

		if (line[0] == 'v')
		{
			parseVertex(line);
			continue;
		}

		if (line[0] == '\n')
		{
			// Line is just newline, we'll ignore it but don't have to track it
			continue;
		}

		if (line[0] == '#')
		{
			// Line is comment, but is still valid. So we ignore it but don't track it as ignored line
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

	for (int i = 1; i < line.size(); ++i)
	{
		char byte = line[i];

		// More than 3 vertices in instruction which is not valid
		if (valueIndex > 2)
		{
			m_ignoredLines++;
			return;
		}

		// If current byte is a whitespace or we are at end of line
		if (std::isspace(byte) > 0 || i == line.size() - 1)
		{
			if (tokenStart < lastWhitespace && (i == line.size() - 1))
			{
				tokenStart = i;
			}

			// tokenStart is greater than lastWhitespace only if we are currently parsing a token
			if (tokenStart > lastWhitespace)
			{
				std::string token = std::string(line.begin() + tokenStart, line.begin() + i + 1);

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

			lastWhitespace = i;
			continue;
		}

		if (tokenStart < lastWhitespace)
		{
			tokenStart = i;
		}
	}

	std::stringstream ss;
	ss << line;
	ss << " Point(" << values[0] << ", " << values[1] << ", " << values[2] << ")\n";
	std::cout << ss.str();

	m_vertices.push_back(Math::Point(values[0], values[1], values[2]));
}

void WavefrontOBJSerializer::parseFace(std::string line)
{
	std::vector<int> indices;

	int lastWhitespace = 0;
	int tokenStart = 0;

	for (int j = 1; j < line.size(); ++j)
	{
		char byte = line[j];

		// If current byte is a whitespace or we are at end of line
		if (std::isspace(byte) > 0 || j == line.size() - 1)
		{
			if (tokenStart < lastWhitespace && (j == line.size() - 1))
			{
				tokenStart = j;
			}

			// tokenStart is greater than lastWhitespace only if we are currently parsing a token
			if (tokenStart > lastWhitespace)
			{
				std::string token = std::string(line.begin() + tokenStart, line.begin() + j + 1);

				try
				{
					indices.push_back(std::stoi(token, nullptr));
				}
				catch (const std::invalid_argument& ia)
				{
					std::cerr << "Invalid argument when parsing OBJ file: " << ia.what() << "\n";
					m_ignoredLines++;
					return;
				}
			}

			lastWhitespace = j;
			continue;
		}

		if (tokenStart < lastWhitespace)
		{
			tokenStart = j;
		}
	}

	// We need at least 3 indices to create a face from 3 vertices
	if (indices.size() < 3)
	{
		m_ignoredLines++;
		return;
	}

	for (int i = 0; i < indices.size(); ++i)
	{
		// If index is larger than vertex array size or points to negative number, this instruction is invalid
		if (indices[i] > m_vertices.size() || indices[i] < 1)
		{
			m_ignoredLines++;
			return;
		}
	}

	// Fan triangulation
	for (int i = 1; i < indices.size() - 1; ++i)
	{
 		auto triangle = std::make_shared<Math::Triangle>(m_vertices[indices.at(0) - 1], m_vertices[indices.at(i) - 1], m_vertices[indices.at(i + 1) - 1]);
		auto shape = std::make_shared<Renderer::Shape>(triangle);
 
		std::stringstream ss;
		ss << line;
		ss << " Triangle([" << triangle->p1() << "], [" << triangle->p2() << "], [" << triangle->p3() << "])\n";
		std::cout << ss.str();

		if (m_currentGroupName == "")
		{
			m_defaultGroup.addChild(shape);
		}
		else
		{
			m_groups.at(m_currentGroupName)->addChild(shape);
		}
	}
}

void WavefrontOBJSerializer::parseGroup(std::string line)
{
	int stringBeginIndex = 1;
	int stringEndIndex = (int)line.size() - 1;

	// Find string begin
	for (int i = 1; i < line.size(); ++i)
	{
		char byte = line[i];

		// If the byte is not a space, set begin index and quit
		if (std::isspace(byte) <= 0)
		{
			stringBeginIndex = i;
			break;
		}
	}

	// Find string end
	for (int i = (int)line.size() - 1; i >= 0; --i)
	{
		char byte = line[i];

		// If the byte is not a space, set end index and quit
		if (std::isspace(byte) <= 0)
		{
			stringEndIndex = i + 1;
			break;
		}
	}

	std::string groupName = std::string({ line.begin() + stringBeginIndex, line.begin() + stringEndIndex });

	auto group = std::make_shared<Renderer::Group>();

	m_groups.insert({ groupName, group });
	m_currentGroupName = groupName;

	m_defaultGroup.addChild(group);

	return;
}

void WavefrontOBJSerializer::parseNormal(std::string line)
{
	double values[3] = { -123, -312, -231 };
	int valueIndex = 0;

	int lastWhitespace = 0;
	int tokenStart = 0;

	for (int i = 1; i < line.size(); ++i)
	{
		char byte = line[i];

		// More than 3 vertices in instruction which is not valid
		if (valueIndex > 2)
		{
			m_ignoredLines++;
			return;
		}

		// If current byte is a whitespace or we are at end of line
		if (std::isspace(byte) > 0 || i == line.size() - 1)
		{
			if (tokenStart < lastWhitespace && (i == line.size() - 1))
			{
				tokenStart = i;
			}

			// tokenStart is greater than lastWhitespace only if we are currently parsing a token
			if (tokenStart > lastWhitespace)
			{
				std::string token = std::string(line.begin() + tokenStart, line.begin() + i + 1);

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

			lastWhitespace = i;
			continue;
		}

		if (tokenStart < lastWhitespace)
		{
			tokenStart = i;
		}
	}

	std::stringstream ss;
	ss << line;
	ss << " Vector(" << values[0] << ", " << values[1] << ", " << values[2] << ")\n";
	std::cout << ss.str();

	m_normals.push_back(Math::Vector(values[0], values[1], values[2]));
}

std::vector<char> WavefrontOBJSerializer::buffer() const
{
	return m_buffer;
}

Renderer::Group WavefrontOBJSerializer::defaultGroup() const
{
	return m_defaultGroup;
}

std::shared_ptr<Renderer::Group> WavefrontOBJSerializer::group(std::string groupName) const
{
	return m_groups.at(groupName);
}

std::vector<Math::Point> WavefrontOBJSerializer::vertices() const
{
	return m_vertices;
}

std::vector<Math::Vector> WavefrontOBJSerializer::normals() const
{
	return m_normals;
}

int WavefrontOBJSerializer::ignoredLines() const
{
	return m_ignoredLines;
}
