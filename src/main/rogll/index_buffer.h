#pragma once

namespace ROGLL
{
	class IndexBuffer
	{
	private:
		unsigned int m_bufferId;
		unsigned int m_count;
	public:
		IndexBuffer(unsigned int* data, const int size);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount() const;
	};
}
