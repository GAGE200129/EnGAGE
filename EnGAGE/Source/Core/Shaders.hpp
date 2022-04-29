#pragma once

#include "Shader.hpp"

namespace Core
{
	class AmbientShader : public Shader
	{
	public:
		AmbientShader();

		void uploadAmbient(const glm::vec3& v);
	private:
		int mAmbientLoc;
	};

	class DirectionalShader : public Shader
	{
	public:
		DirectionalShader();

		void uploadParams(const glm::vec3& dir, const glm::vec3& color, float intensity, const glm::vec3& camPos);
	private:
		int mDirectionLoc, mColorLoc, mIntensityLoc, mCamPosLoc;
	};

	class PointShader : public Shader
	{
	public:
		PointShader();
	
		void uploadParams(const glm::vec3& color, const glm::vec3& position, float intensity, float constant, float linear, float exponent, const glm::vec3& camPos);
	private:
		int mColor, mPosition, mIntensity, mConstant, mLinear, mExponent, mCamPos;
	};
}