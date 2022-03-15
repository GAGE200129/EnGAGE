#include "pch.hpp"
#include "Shader.hpp"

#include <glad/glad.h>

namespace Core
{
	Shader::Shader() :
		mProgramID(glCreateProgram())
	{
		for (unsigned int i = 0u; i < (unsigned int)ShaderType::COUNT; i++)
		{
			mShaders[i] = 0;
		}
	}

	void Shader::cleanup()
	{
		glDeleteProgram(mProgramID);
	}


	void Shader::loadVertexShader(const String& filePath)
	{
		loadSource(filePath, ShaderType::VERTEX);
	}

	void Shader::loadFragmentShader(const String& filePath)
	{
		loadSource(filePath, ShaderType::FRAGMENT);
	}

	void Shader::compile()
	{
		for (unsigned int i = 0; i < (unsigned int)ShaderType::COUNT; i++)
		{
			glAttachShader(mProgramID, mShaders[i]);
		}
		glLinkProgram(mProgramID);
		glValidateProgram(mProgramID);

		for (unsigned int i = 0; i < (unsigned int)ShaderType::COUNT; i++)
		{
			glDeleteShader(mShaders[i]);
		}
	}

	void Shader::bind() const
	{
		glUseProgram(mProgramID);
	}

	void Shader::loadSource(const String& filePath, ShaderType type)
	{
		uint32_t shader = glCreateShader(getShaderTypeEnum(type));
		std::ifstream is(filePath);
		EN_ASSERT(is.is_open(), "Failed to read file: {}", filePath.c_str());
		String source(std::istreambuf_iterator<char>(is), {});
		const char* source_c_str = source.c_str();
		glShaderSource(shader, 1, &source_c_str, nullptr);
		glCompileShader(shader);
		int status = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			std::unique_ptr<char[]> message;
			int logLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			message = std::make_unique<char[]>(logLength);
			glGetShaderInfoLog(shader, logLength, &logLength, message.get());
			EN_ERROR("Failed to compile shader, reason: {}", message.get());
		}
		mShaders[(unsigned int)type] = shader;
	}

	uint32_t Shader::getShaderTypeEnum(ShaderType type)
	{
		switch (type)
		{
		case Core::Shader::ShaderType::VERTEX:
			return GL_VERTEX_SHADER;
			break;
		case Core::Shader::ShaderType::FRAGMENT:
			return GL_FRAGMENT_SHADER;
			break;
		}
		EN_ASSERT(false, "Unknown shader type!");
	}

}
