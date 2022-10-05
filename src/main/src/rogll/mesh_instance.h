#pragma once
#include <RML.h>
#include "mesh.h"

namespace ROGLL
{
	class MeshInstance
	{
	public:
		MeshInstance(const Mesh& mesh);
		MeshInstance(const Mesh& mesh, RML::Transform transform);

		unsigned int vertexCount() const;
		std::vector<unsigned int> indexData() const;
		std::vector<float> vertexData() const;
	private:
		const Mesh& m_mesh;
		mutable bool m_validCache;
		mutable std::vector<float> m_cachedVertexData;
		mutable RML::Transform m_lastTransform;
	public:
		RML::Transform transform;
	};
}
