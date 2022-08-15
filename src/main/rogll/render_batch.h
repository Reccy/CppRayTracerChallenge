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

		void Render(const Camera& cam) const;
	private:
		std::set<const MeshInstance*> m_meshInstances;
		const VertexAttributes* const m_layout;
		Material* const m_material;
	};
}
