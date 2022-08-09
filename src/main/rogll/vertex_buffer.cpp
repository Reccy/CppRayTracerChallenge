#include "vertex_buffer.h"
#include <glad.h>
#include <iostream>

namespace ROGLL
{
	VertexBuffer::VertexBuffer(const void* data, const int size)
	{
		glGenBuffers(1, &m_bufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_bufferId);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
