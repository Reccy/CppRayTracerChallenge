#pragma once
#include <string>

namespace ROGLL
{
	class Shader
	{
	private:
		unsigned int m_shaderId;
	public:
		Shader(const std::string& sourceFilepath);

		void Bind() const;
		void Unbind() const;

		int GetUniformLocation(const std::string& name) const;
	};
}
