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

		void cleanup();

		void loadVertexShader(const String& filePath);
		void loadFragmentShader(const String& filePath);
		void compile();
		void bind() const;
	private:
		void loadSource(const String& filePath, ShaderType type);
		uint32_t getShaderTypeEnum(ShaderType type);
	private:
	
		uint32_t mProgramID;
		uint32_t mShaders[(unsigned int)ShaderType::COUNT];
	};
}