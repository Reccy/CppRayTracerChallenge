#include "vertex_array.h"
#include <glad.h>

#include <iostream>

namespace ROGLL
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_arrayId);
		glBindVertexArray(m_arrayId);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_arrayId);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_arrayId);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::SetBuffer(const VertexAttributes& attributes, const VertexBuffer& buffer)
	{
		Bind();
		buffer.Bind();

		auto attributesVector = attributes.GetAttributes();

		unsigned int offsetAccumulator = 0;

		for (GLuint i = 0; i < attributesVector.size(); i++)
		{
			auto attribute = attributesVector[i];
			
			glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalized, attributes.GetStride(), (void*)offsetAccumulator);
			glEnableVertexAttribArray(i);

			offsetAccumulator += attribute.count * attribute.size;
		}
	}
}
