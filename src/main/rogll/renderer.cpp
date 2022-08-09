#include "renderer.h"

namespace ROGLL
{
	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetClearColor(const RML::Tuple4<float>& color)
	{
		glClearColor(color.x(), color.y(), color.z(), color.w());
	}

	void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Material& material)
	{
		vertexArray.Bind();
		indexBuffer.Bind();
		material.Bind();

		glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, (void*)0);

		material.Unbind();
		indexBuffer.Unbind();
		vertexArray.Unbind();
	}
}
