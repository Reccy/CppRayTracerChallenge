#pragma once
#include "vertex_attributes.h"
#include "vertex_buffer.h"

namespace ROGLL
{
	class VertexArray
	{
	private:
		unsigned int m_arrayId;
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void SetBuffer(const VertexAttributes& attributes, const VertexBuffer& buffer);
	};
}
