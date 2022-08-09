#include "index_buffer.h"
#include <glad.h>

namespace ROGLL
{
	IndexBuffer::IndexBuffer(unsigned int* data, const int size)
		: m_count(size / sizeof(unsigned int))
	{
		glGenBuffers(1, &m_bufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_bufferId);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int IndexBuffer::GetCount() const
	{
		return m_count;
	}
}
