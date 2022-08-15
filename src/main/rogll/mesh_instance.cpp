#include "mesh_instance.h"

namespace ROGLL
{
	MeshInstance::MeshInstance(const Mesh& mesh) :
		m_mesh(mesh)
	{}

	MeshInstance::MeshInstance(const Mesh& mesh, RML::Transform transform) :
		m_mesh(mesh),
		transform(transform)
	{}

	unsigned int MeshInstance::vertexCount() const
	{
		std::vector<float> result = m_mesh.vertexData();
		unsigned int stride = m_mesh.attributes().GetStride();
		return (result.size() / (stride / 4));
	}

	std::vector<unsigned int> MeshInstance::indexData() const
	{
		return m_mesh.indexData();
	}

	std::vector<float> MeshInstance::vertexData() const
	{
		std::vector<float> result = m_mesh.vertexData();

		const VertexAttributes& attributes = m_mesh.attributes();
		VertexAttributes::VertexAttribute attribute;
		unsigned int attributeOffset;
		if (!m_mesh.attributes().GetTaggedAttribute(VertexAttributes::POSITION, attribute, attributeOffset))
		{
			std::cout << "ERROR: Could not find POSITION attribute" << std::endl;
			std::cout << "ERROR: Returning empty array" << std::endl;
			return result;
		}

		unsigned int stride = attributes.GetStride();
		unsigned int vertCount = (result.size() / (stride / 4));

		for (size_t i = 0; i < vertCount; i++)
		{
			unsigned int offset = i * (stride / 4) + attributeOffset;

			RML::Matrix<double, 4, 1> positionMatrix({
				result[offset],
				result[offset + 1],
				result[offset + 2],
				1
			});

			auto positionResult = transform.matrix() * positionMatrix;

			result[offset] = positionResult(0, 0);
			result[offset + 1] = positionResult(0, 1);
			result[offset + 2] = positionResult(0, 2);
		}

		return result;
	}
}
