#include "material.h"
#include <glad.h>
#include <functional>

namespace ROGLL
{
	Material::Material(const Shader& shader)
		: m_shader(shader),
		m_uniformsF4x4(),
		m_uniformsF4(),
		m_uniformsF3(),
		m_uniformsF2(),
		m_uniformsF1(),
		m_uniformsI4(),
		m_uniformsI3(),
		m_uniformsI2(),
		m_uniformsI1(),
		m_uniformsUI4(),
		m_uniformsUI3(),
		m_uniformsUI2(),
		m_uniformsUI1()
	{}

	template<typename T>
	void BindUniformsT1(std::unordered_map<int, T> map, std::function<void(int, T)> fn)
	{
		for (const auto& pair : map)
		{
			const auto& val = pair.second;
			fn(pair.first, val);
		}
	}

	template<typename T>
	void BindUniformsT2(std::unordered_map<int, RML::Tuple2<T>> map, std::function<void(int, T, T)> fn)
	{
		for (const auto& pair : map)
		{
			const auto& vec = pair.second;
			fn(pair.first, vec.x(), vec.y());
		}
	}

	template<typename T>
	void BindUniformsT3(std::unordered_map<int, RML::Tuple3<T>> map, std::function<void(int, T, T, T)> fn)
	{
		for (const auto& pair : map)
		{
			const auto& vec = pair.second;
			fn(pair.first, vec.x(), vec.y(), vec.z());
		}
	}

	template<typename T>
	void BindUniformsT4(std::unordered_map<int, RML::Tuple4<T>> map, std::function<void(int, T, T, T, T)> fn)
	{
		for (const auto& pair : map)
		{
			const auto& vec = pair.second;
			fn(pair.first, vec.x(), vec.y(), vec.z(), vec.w());
		}
	}

	void BindUniformsT4x4F(std::unordered_map<int, Matrix4x4<float>> map)
	{
		for (const auto& pair : map)
		{
			const auto& matrix = pair.second;
			glUniformMatrix4fv(pair.first, 1, true , matrix.data());
		}
	}

	void Material::Bind() const
	{
		m_shader.Bind();

		BindUniformsT4x4F(m_uniformsF4x4);
		BindUniformsT4<float>(m_uniformsF4, glUniform4f);
		BindUniformsT3<float>(m_uniformsF3, glUniform3f);
		BindUniformsT2<float>(m_uniformsF2, glUniform2f);
		BindUniformsT1<float>(m_uniformsF1, glUniform1f);

		BindUniformsT4<int>(m_uniformsI4, glUniform4i);
		BindUniformsT3<int>(m_uniformsI3, glUniform3i);
		BindUniformsT2<int>(m_uniformsI2, glUniform2i);
		BindUniformsT1<int>(m_uniformsI1, glUniform1i);

		BindUniformsT4<unsigned int>(m_uniformsUI4, glUniform4ui);
		BindUniformsT3<unsigned int>(m_uniformsUI3, glUniform3ui);
		BindUniformsT2<unsigned int>(m_uniformsUI2, glUniform2ui);
		BindUniformsT1<unsigned int>(m_uniformsUI1, glUniform1ui);
	}

	void Material::Unbind() const
	{
		m_shader.Unbind();
	}

	void Material::Set4x4(const std::string& name, Matrix4x4<float> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsF4x4[location] = value;
	}

	void Material::Set4(const std::string& name, RML::Tuple4<int> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsI4[location] = value;
	}

	void Material::Set4(const std::string& name, RML::Tuple4<unsigned int> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsUI4[location] = value;
	}

	void Material::Set4(const std::string& name, RML::Tuple4<float> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsF4[location] = value;
	}

	void Material::Set3(const std::string& name, RML::Tuple3<int> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsI3[location] = value;
	}

	void Material::Set3(const std::string& name, RML::Tuple3<unsigned int> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsUI3[location] = value;
	}

	void Material::Set3(const std::string& name, RML::Tuple3<float> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsF3[location] = value;
	}

	void Material::Set2(const std::string& name, RML::Tuple2<int> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsI2[location] = value;
	}

	void Material::Set2(const std::string& name, RML::Tuple2<unsigned int> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsUI2[location] = value;
	}

	void Material::Set2(const std::string& name, RML::Tuple2<float> value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsF2[location] = value;
	}

	void Material::Set(const std::string& name, int value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsI1[location] = value;
	}

	void Material::Set(const std::string& name, unsigned int value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsUI1[location] = value;
	}

	void Material::Set(const std::string& name, float value)
	{
		int location = m_shader.GetUniformLocation(name);
		m_uniformsF1[location] = value;
	}
}
