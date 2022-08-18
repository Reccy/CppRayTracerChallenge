#include "shader.h"
#include <glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

struct ShaderProgramSource
{
	std::string vertex;
	std::string fragment;
	bool success = false;
};

enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
static const char* ShaderTypeStrings[2] = { "Vertex", "Fragment" };
static const int ShaderTypeConsts[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

static ShaderProgramSource LoadShaderSource(std::string filepath)
{
	ShaderProgramSource result;

	std::ifstream infile;
	infile.open(filepath);

	if (!infile.is_open())
	{
		std::cout << "\n[File Load Error] Failed to load file at " << filepath << std::endl;
		return result;
	}

	std::stringstream sourceStream[2];

	enum ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
	ShaderType type = NONE;

	std::string line;
	while (std::getline(infile, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			sourceStream[(int)type] << line << '\n';
		}
	}

	result.vertex = sourceStream[(int)ShaderType::VERTEX].str();
	result.fragment = sourceStream[(int)ShaderType::FRAGMENT].str();
	result.success = true;

	return result;
}

static bool ValidateShaderCompiled(unsigned int shader)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		int size = 256;
		char* infoLog = (char*)_malloca(sizeof(char*) * size);
		glGetShaderInfoLog(shader, size, &size, infoLog);

		std::cout << "\n[OpenGL Shader Compile Error] " << infoLog << std::endl;

		_freea(infoLog);
	}

	return success;
}

static bool ValidateShaderProgramLinked(unsigned int program)
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int size = 256;
		char* infoLog = (char*)_malloca(sizeof(char*) * size);
		glGetProgramInfoLog(program, size, &size, infoLog);

		std::cout << "\n[OpenGL Program Link Error] " << infoLog << std::endl;

		_freea(infoLog);
	}

	return success;
}

static unsigned int CompileShader(const std::string& source, ShaderType type)
{
	const char* str = source.c_str();

	unsigned int shader = glCreateShader(ShaderTypeConsts[(int)type]);
	glShaderSource(shader, 1, &str, NULL);
	glCompileShader(shader);

	if (!ValidateShaderCompiled(shader))
	{
		std::cout << "\nFailed to compile shader (Type: " << ShaderTypeStrings[(int)type] << ")" << std::endl;
		return -1;
	}

	return shader;
}

static unsigned int CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	unsigned int vertexShader = CompileShader(vertexShaderSource, ShaderType::VERTEX);
	unsigned int fragmentShader = CompileShader(fragmentShaderSource, ShaderType::FRAGMENT);

	if (vertexShader == -1 || fragmentShader == -1)
	{
		return -1;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	if (!ValidateShaderProgramLinked(shaderProgram))
	{
		std::cout << "\nFailed to link shader program" << std::endl;
		return -1;
	}

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

namespace ROGLL
{
	Shader::Shader(const std::string& sourceFilepath)
		: m_shaderId(0)
	{
		std::cout << "Loading Shader " << sourceFilepath << "... ";
		ShaderProgramSource shaderSource = LoadShaderSource(sourceFilepath);

		if (!shaderSource.success)
		{
			std::cout << "Failed to load shader!" << std::endl;
			return;
		}

		m_shaderId = CreateShaderProgram(shaderSource.vertex, shaderSource.fragment);

		if (m_shaderId == -1)
		{
			std::cout << "Failed to create shader program!" << std::endl;
			return;
		}

		std::cout << "Done" << std::endl;
	}

	void Shader::Bind() const
	{
		glUseProgram(m_shaderId);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	int Shader::GetUniformLocation(const std::string& name) const
	{
		Bind();
		return glGetUniformLocation(m_shaderId, name.c_str());
	}
}
