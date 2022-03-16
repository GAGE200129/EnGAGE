#pragma once

#include "pch.hpp"

namespace Core
{
	class Shader
	{
		enum class ShaderType
		{
			VERTEX,
			FRAGMENT,
			COUNT
		};
	public:
		Shader();
		virtual ~Shader() = default;

		void cleanup();

		void loadVertexShader(const String& filePath);
		void loadFragmentShader(const String& filePath);
		void compile();
		int registerUniform(const String& name);
		void uploadMat4x4(int loc, const glm::mat4x4& mat);
		void bind() const;
	private:
		void loadSource(const String& filePath, ShaderType type);
		uint32_t getShaderTypeEnum(ShaderType type);
	private:
	
		uint32_t mProgramID;
		uint32_t mShaders[(unsigned int)ShaderType::COUNT];
	};
}