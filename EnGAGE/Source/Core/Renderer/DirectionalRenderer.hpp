#pragma once

#include "Core/Shader.hpp"
#include "GBuffer.hpp"
#include "MapRenderer.hpp"
#include "EntityRenderer.hpp"
#include "Core/Data/Camera.hpp"

namespace Core
{
	class DirectionalShader : public Shader
	{
	public:
		DirectionalShader();

		void uploadParams(const Vec3& dir, const Vec3& color, float intensity, const Vec3& camPos);
		void uploadShadowProjView(const Mat4x4& projView);
		void uploadShadowShadowDistanceAndFadeStart(F32 distance, F32 fadeStart);
	private:
		int mDirectionLoc, mColorLoc, mIntensityLoc, mCamPosLoc, mLightProjView, mShadowDistance, mShadowFadeStart;
	};

	class ShadowMapShader : public Shader
	{
	public:
		ShadowMapShader();

		void uploadProjView(const Mat4x4& projView);
		void uploadModel(const Mat4x4& modelMat);
	private:
		int mProjViewLoc, mModelMat;
	};

	class DirectionalRenderer
	{
	public:
		DirectionalRenderer(UInt32 shadowSize);
		~DirectionalRenderer();
		void render(UInt32 width, UInt32 height, GBuffer& gBuffer, const Camera& camera, MapRenderer& mapRenderer, F32 shadowDistance, F32 shadowFadeStart);
		void resize(UInt32 shadowSize);
		inline UInt32 getDepthMap() { return mDepthMap; }
	private:
		DirectionalShader mShader;
		ShadowMapShader mShadowShader;
		UInt32 mFBO, mDepthMap;
		UInt32 mShadowSize;
	};
}