#pragma once

#include "Core/Shader.hpp"

namespace Core
{
	class PointShader : public Shader
	{
	public:
		PointShader();

		void uploadParams(const glm::vec3& color, const glm::vec3& position, float intensity, float constant, float linear, float exponent, const glm::vec3& camPos);
	private:
		int mColor, mPosition, mIntensity, mConstant, mLinear, mExponent, mCamPos;
	};

	class PointRenderer
	{
	public:
		PointRenderer();
		~PointRenderer();
		void render(const glm::vec3& color, const glm::vec3& position, float intensity, float constant, float linear, float exponent, const glm::vec3& camPos);
	private:
		PointShader mShader;
	};
}