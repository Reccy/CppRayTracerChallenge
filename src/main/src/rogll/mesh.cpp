#include "mesh.h"

namespace ROGLL
{
	Mesh::Mesh(std::vector<float> verts, std::vector<unsigned int> indices, const VertexAttributes& attributes) :
		m_verts(verts),
		m_indices(indices),
		m_attributes(attributes)
	{}

	std::vector<float> Mesh::vertexData() const
	{
		return m_verts;
	}

	std::vector<unsigned int> Mesh::indexData() const
	{
		return m_indices;
	}

	const VertexAttributes& Mesh::attributes() const
	{
		return m_attributes;
	}
}
