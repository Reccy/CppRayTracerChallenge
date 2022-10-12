#pragma once

namespace ROGLL
{
	class VertexBuffer
	{
	private:
		unsigned int m_bufferId;
	public:
		VertexBuffer(const void* data, const int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	};
}
