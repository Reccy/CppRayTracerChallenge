#pragma once
#include "vertex_attributes.h"

namespace ROGLL
{
	class Mesh
	{
	public:
		Mesh(std::vector<float> verts, std::vector<unsigned int> indices, const VertexAttributes& attributes);
	
		std::vector<float> vertexData() const;
		std::vector<unsigned int> indexData() const;
		const VertexAttributes& attributes() const;
	private:
		std::vector<float> m_verts;
		std::vector<unsigned int> m_indices;
		const VertexAttributes& m_attributes;
	};
}
