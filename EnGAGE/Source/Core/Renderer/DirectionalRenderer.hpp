#pragma once

#include "Core/Shader.hpp"

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
		void render(const Vec3& dir, const Vec3& color, float intensity, const Vec3& camPos);
	private:
		DirectionalShader mShader;
	};
}