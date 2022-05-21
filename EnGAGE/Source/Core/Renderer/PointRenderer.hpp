#pragma once

#include "Core/Shader.hpp"
#include "GBuffer.hpp"
#include "Core/Camera.hpp"

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
		void render(const GBuffer& gBuffer, const Camera& camera);
	private:
		PointShader mShader;
	};
}