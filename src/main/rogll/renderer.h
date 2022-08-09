#pragma once
#include "vertex_array.h"
#include "index_buffer.h"
#include "material.h"
#include <RML.h>

namespace ROGLL
{
	class Renderer
	{
	public:
		void Clear();
		void SetClearColor(const RML::Tuple4<float>& color);
		void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Material& material);
	};
}
