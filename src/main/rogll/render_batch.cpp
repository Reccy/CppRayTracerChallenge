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

	void RenderBatch::Clear()
	{
		m_meshInstances.clear();
	}

	void RenderBatch::Render(const Camera& cam, const RML::Tuple3<float>& lightPosition) const
	{
		std::vector<unsigned int> indexBuffer;
		std::vector<float> vertexBuffer;

		unsigned int indexBufferOffset = 0;
		
		for (const MeshInstance* const instance : m_meshInstances)
		{
			const auto& verts = instance->vertexData();
			const auto& indices = instance->indexData();

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

		m_material->Set4x4("uVP", cam.GetProjectionMatrix() * cam.GetViewMatrix());
		m_material->Set3("lightPos", lightPosition);
		m_material->Set3("lightColor", RML::Tuple3<float>(1.0, 1.0, 1.0));
		m_material->Set3("viewPos", RML::Tuple3<float>(cam.transform.position.x(), cam.transform.position.y(), cam.transform.position.z()));
		m_material->BindAndApplyUniforms();

		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, (void*)0);
	}
}
