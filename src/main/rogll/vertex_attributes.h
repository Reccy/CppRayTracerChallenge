#pragma once
#include <glad.h>
#include <vector>

namespace ROGLL
{
	class VertexAttributes
	{
	private:
		struct VertexAttribute
		{
			GLsizei count;
			GLenum type;
			GLboolean normalized;
			unsigned int size;
		};

		GLsizei m_stride;
		std::vector<VertexAttribute> m_attributes;
	public:
		VertexAttributes();

		template<typename T>
		void Add(GLsizei count, GLboolean normalized = false);

		template<>
		void Add<float>(GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ count, GL_FLOAT, normalized, sizeof(float) });
			m_stride += sizeof(GLfloat) * count;
		}

		template<>
		void Add<int>(GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ count, GL_INT, normalized, sizeof(int) });
			m_stride += sizeof(GLint) * count;
		}

		template<>
		void Add<unsigned int>(GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ count, GL_UNSIGNED_INT, normalized, sizeof(unsigned int) });
			m_stride += sizeof(GLuint) * count;
		}

		template<>
		void Add<char>(GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ count, GL_BYTE, normalized, sizeof(char) });
			m_stride += sizeof(GLbyte) * count;
		}

		template<>
		void Add<unsigned char>(GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ count, GL_UNSIGNED_BYTE, normalized, sizeof(unsigned char) });
			m_stride += sizeof(GLubyte) * count;
		}

		template<>
		void Add<short>(GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ count, GL_SHORT, normalized, sizeof(short) });
			m_stride += sizeof(GLshort) * count;
		}

		template<>
		void Add<unsigned short>(GLsizei count, GLboolean normalized)
		{
			m_attributes.push_back({ count, GL_UNSIGNED_SHORT, normalized, sizeof(unsigned short) });
			m_stride += sizeof(GLushort) * count;
		}

		const std::vector<VertexAttribute>& GetAttributes() const;
		const GLsizei GetStride() const;
	};
}
