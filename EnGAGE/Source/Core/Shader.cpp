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
		EN_INFO("Deleting shader: {}", mProgramID);
		glDeleteProgram(mProgramID);
	}


	void Shader::loadVertexShader(const String& filePath)
	{
		loadSource(filePath, ShaderType::VERTEX);
	}

	void Shader::loadGeometryShader(const String& filePath)
	{
		loadSource(filePath, ShaderType::GEOMETRY);
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
		EN_INFO("Compiling shader: {}", mProgramID);
	}

	int Shader::registerUniform(const String& name)
	{
		return glGetUniformLocation(mProgramID, name.c_str());
	}

	void Shader::uploadMat4x4(int loc, const glm::mat4x4& mat) const
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::uploadInt(int loc, int v)
	{
		glUniform1i(loc, v);
	}

	void Shader::uploadFloat(int loc, float v)
	{
		glUniform1f(loc, v);
	}

	void Shader::uploadVec3(int loc, const glm::vec3& v)
	{
		glUniform3f(loc, v.x, v.y, v.z);
	}

	void Shader::bind() const
	{
		glUseProgram(mProgramID);
	}

	void Shader::unBind() const
	{
		glUseProgram(0);
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
		case Core::Shader::ShaderType::GEOMETRY:
			return GL_GEOMETRY_SHADER;
			break;
		case Core::Shader::ShaderType::FRAGMENT:
			return GL_FRAGMENT_SHADER;
			break;
		}
		EN_ASSERT(false, "Unknown shader type!");
		return 0;
	}

}
