#pragma once
#include "shader.h"
#include "types.h"
#include <RML.h>
#include <iostream>
#include <unordered_map>

namespace ROGLL
{
	class Material
	{
	private:
		template<typename T>
		using Mat4x4Uniforms = std::unordered_map<int, Matrix4x4<T>>;

		template<typename T>
		using Vec4Uniforms = std::unordered_map<int, RML::Tuple4<T>>;

		template<typename T>
		using Vec3Uniforms = std::unordered_map<int, RML::Tuple3<T>>;

		template<typename T>
		using Vec2Uniforms = std::unordered_map<int, RML::Tuple2<T>>;

		template<typename T>
		using Vec1Uniforms = std::unordered_map<int, T>;
	public:
		Material(const Shader& shader);

		void BindAndApplyUniforms() const;
		void Unbind() const;

		void Set4x4(const std::string& name, Matrix4x4<float> value);

		void Set4(const std::string& name, RML::Tuple4<int> value);
		void Set4(const std::string& name, RML::Tuple4<unsigned int> value);
		void Set4(const std::string& name, RML::Tuple4<float> value);

		void Set3(const std::string& name, RML::Tuple3<int> value);
		void Set3(const std::string& name, RML::Tuple3<unsigned int> value);
		void Set3(const std::string& name, RML::Tuple3<float> value);

		void Set2(const std::string& name, RML::Tuple2<int> value);
		void Set2(const std::string& name, RML::Tuple2<unsigned int> value);
		void Set2(const std::string& name, RML::Tuple2<float> value);

		void Set(const std::string& name, int value);
		void Set(const std::string& name, unsigned int value);
		void Set(const std::string& name, float value);
	private:
		const Shader& m_shader;

		Mat4x4Uniforms<float> m_uniformsF4x4;
		Vec4Uniforms<float> m_uniformsF4;
		Vec3Uniforms<float> m_uniformsF3;
		Vec2Uniforms<float> m_uniformsF2;
		Vec1Uniforms<float> m_uniformsF1;

		Vec4Uniforms<int> m_uniformsI4;
		Vec3Uniforms<int> m_uniformsI3;
		Vec2Uniforms<int> m_uniformsI2;
		Vec1Uniforms<int> m_uniformsI1;

		Vec4Uniforms<unsigned int> m_uniformsUI4;
		Vec3Uniforms<unsigned int> m_uniformsUI3;
		Vec2Uniforms<unsigned int> m_uniformsUI2;
		Vec1Uniforms<unsigned int> m_uniformsUI1;
	};
}
