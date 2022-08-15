#include "render_batch.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "vertex_buffer.h"

namespace ROGLL
{
	RenderBatch::RenderBatch(const VertexAttributes* const layout, Material* const material) :
		m_layout(layout),
		m_material(material)
	{}

	void RenderBatch::AddInstance(const MeshInstance* instance)
	{
		m_meshInstances.insert(instance);
	}

	void RenderBatch::RemoveInstance(const MeshInstance* instance)
	{
		m_meshInstances.erase(instance);
	}

	void RenderBatch::Render(const Camera& cam) const
	{
		std::vector<unsigned int> indexBuffer;
		std::vector<float> vertexBuffer;

		unsigned int indexBufferOffset = 0;
		
		for (const MeshInstance* const instance : m_meshInstances)
		{
			auto verts = instance->vertexData();
			auto indices = instance->indexData();

			for (const unsigned int index : indices)
			{
				indexBuffer.push_back(index + indexBufferOffset);
			}
			indexBufferOffset += instance->vertexCount();

			for (const float vertex : verts)
			{
				vertexBuffer.push_back(vertex);
			}
		}

		ROGLL::VertexArray vao;
		ROGLL::IndexBuffer ibo(indexBuffer.data(), indexBuffer.size());
		ROGLL::VertexBuffer vbo(vertexBuffer.data(), vertexBuffer.size());
		vao.SetBuffer(*m_layout, vbo);

		vao.Bind();
		vbo.Bind();
		ibo.Bind();

		m_material->Bind();
		m_material->Set4x4("uVP", cam.GetProjectionMatrix() * cam.GetViewMatrix());

		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, (void*)0);
	}
}
