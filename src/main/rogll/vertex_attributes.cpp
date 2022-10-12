#include "vertex_attributes.h"

namespace ROGLL
{
	VertexAttributes::VertexAttributes()
		: m_stride(0)
	{}

	const std::vector<VertexAttributes::VertexAttribute>& VertexAttributes::GetAttributes() const
	{
		return m_attributes;
	}

	const GLsizei VertexAttributes::GetStride() const
	{
		return m_stride;
	}
}
