#include "shader.h"
#include <glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

struct ShaderProgramSource
{
	std::string vertex;
	std::string fragment;
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
		std::cout << "[File Load Error] Failed to load file at " << filepath << std::endl;
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

		std::cout << "[OpenGL Shader Compile Error] " << infoLog << std::endl;

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

		std::cout << "[OpenGL Program Link Error] " << infoLog << std::endl;

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
		std::cout << "Failed to compile shader (Type: " << ShaderTypeStrings[(int)type] << ")" << std::endl;
		return -1;
	}

	std::cout << "Successfully compiled shader (Type: " << ShaderTypeStrings[(int)type] << ")" << std::endl;
	return shader;
}

static unsigned int CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	unsigned int vertexShader = CompileShader(vertexShaderSource, ShaderType::VERTEX);
	unsigned int fragmentShader = CompileShader(fragmentShaderSource, ShaderType::FRAGMENT);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	if (!ValidateShaderProgramLinked(shaderProgram))
	{
		std::cout << "Failed to link shader program" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Successfully linked shader program" << std::endl;
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
		ShaderProgramSource shaderSource = LoadShaderSource(sourceFilepath);
		m_shaderId = CreateShaderProgram(shaderSource.vertex, shaderSource.fragment);
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
