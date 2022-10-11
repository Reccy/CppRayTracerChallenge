#pragma once
#include <set>
#include "mesh_instance.h"
#include "material.h"
#include "vertex_attributes.h"
#include "camera.h"

namespace ROGLL
{
	class RenderBatch
	{
	public:
		RenderBatch(const VertexAttributes* const layout, Material* const material);

		void AddInstance(const MeshInstance* instance);
		void RemoveInstance(const MeshInstance* instance);
		void Clear();

		void Render(const Camera& cam, const RML::Tuple3<float>& lightPosition, const RML::Tuple3<float>& lightColor) const;
	private:
		std::set<const MeshInstance*> m_meshInstances;
		const VertexAttributes* m_layout;
		Material* const m_material;
	};
}
