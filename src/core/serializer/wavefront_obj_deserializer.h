#pragma once
#include <vector>
#include <map>
#include "RML.h"
#include "../renderer/group.h"

namespace CppRayTracerChallenge::Core::Serializer
{
	class WavefrontOBJDeserializer
	{
	public:
		void deserialize(std::vector<char> buffer);

		/// <summary>
		/// Returns the serialized group as a buffer
		/// </summary>
		/// <returns>Group serialized as buffer</returns>
		std::vector<char> buffer() const;

		/// <summary>
		/// Returns the default group
		/// </summary>
		/// <returns>Default group</returns>
		Renderer::Group defaultGroup() const;

		/// <summary>
		/// Returns the group specified by the string, otherwise throws an exception
		/// </summary>
		/// <returns>Group if found</returns>
		std::shared_ptr<Renderer::Group> group(std::string groupName) const;

		/// <summary>
		/// Returns the list of vertices
		/// </summary>
		/// <returns>Point normal list</returns>
		std::vector<RML::Point> vertices() const;

		/// <summary>
		/// Returns the list of normals
		/// </summary>
		/// <returns>Vector normal list</returns>
		std::vector<RML::Vector> normals() const;

		/// <summary>
		/// Returns the amount of lines that were ignored during deserialization
		/// </summary>
		/// <returns>Lines ignored during deserialization</returns>
		int ignoredLines() const;
	private:
		std::vector<char> m_buffer;
		std::vector<RML::Point> m_vertices;
		std::vector<RML::Vector> m_normals;
		Renderer::Group m_defaultGroup;
		std::map<std::string, std::shared_ptr<Renderer::Group>> m_groups;
		std::string m_currentGroupName;

		int m_ignoredLines = 0;

		void parseVertex(std::string line);
		void parseFace(std::string line);
		void parseGroup(std::string line);
		void parseNormal(std::string line);
	};
}
