#pragma once
#include <glad.h>
#include <vector>

namespace ROGLL
{
	class VertexAttributes
	{
	public:
		enum VertexAttributeTag {
			NONE,
			POSITION
		};

		struct VertexAttribute
		{
			VertexAttributeTag tag;
			GLsizei count;
			GLenum type;
			GLboolean normalized;
			unsigned int size;
		};

	private:
		GLsizei m_stride;
		std::vector<VertexAttribute> m_attributes;
	public:
		VertexAttributes();

		bool GetTaggedAttribute(VertexAttributeTag tag, VertexAttribute& outResult, unsigned int& outOffset) const
		{
			unsigned int offset = 0;

			for (const auto& attribute : m_attributes)
			{
				if (attribute.tag == tag)
				{
					outResult = attribute;
					outOffset = offset;
					return true;
				}

				offset += attribute.size;
			}

			return false;
		}

		template<typename T>
		void Add(VertexAttributeTag tag, GLsizei count, GLboolean normalized = false);

		template<>
		void Add<float>(VertexAttributeTag tag, GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ tag, count, GL_FLOAT, normalized, sizeof(float) });
			m_stride += sizeof(GLfloat) * count;
		}

		template<>
		void Add<int>(VertexAttributeTag tag, GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ tag, count, GL_INT, normalized, sizeof(int) });
			m_stride += sizeof(GLint) * count;
		}

		template<>
		void Add<unsigned int>(VertexAttributeTag tag, GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ tag, count, GL_UNSIGNED_INT, normalized, sizeof(unsigned int) });
			m_stride += sizeof(GLuint) * count;
		}

		template<>
		void Add<char>(VertexAttributeTag tag, GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ tag, count, GL_BYTE, normalized, sizeof(char) });
			m_stride += sizeof(GLbyte) * count;
		}

		template<>
		void Add<unsigned char>(VertexAttributeTag tag, GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ tag, count, GL_UNSIGNED_BYTE, normalized, sizeof(unsigned char) });
			m_stride += sizeof(GLubyte) * count;
		}

		template<>
		void Add<short>(VertexAttributeTag tag, GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ tag, count, GL_SHORT, normalized, sizeof(short) });
			m_stride += sizeof(GLshort) * count;
		}

		template<>
		void Add<unsigned short>(VertexAttributeTag tag, GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ tag, count, GL_UNSIGNED_SHORT, normalized, sizeof(unsigned short) });
			m_stride += sizeof(GLushort) * count;
		}

		const std::vector<VertexAttribute>& GetAttributes() const;
		const GLsizei GetStride() const;
	};
}
