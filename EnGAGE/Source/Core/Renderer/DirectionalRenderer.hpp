#pragma once

#include "Core/Shader.hpp"
#include "GBuffer.hpp"
#include "Core/Camera.hpp"

namespace Core
{
	class DirectionalShader : public Shader
	{
	public:
		DirectionalShader();

		void uploadParams(const Vec3& dir, const Vec3& color, float intensity, const Vec3& camPos);
	private:
		int mDirectionLoc, mColorLoc, mIntensityLoc, mCamPosLoc;
	};

	class DirectionalRenderer
	{
	public:
		DirectionalRenderer();
		~DirectionalRenderer();
		void render(const GBuffer& gBuffer, const Camera& camera);
	private:
		DirectionalShader mShader;
	};
}