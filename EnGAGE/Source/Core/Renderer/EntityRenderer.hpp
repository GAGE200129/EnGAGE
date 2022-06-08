#pragma once

#include "Core/Data/Camera.hpp"
#include "GBuffer.hpp"

namespace Core
{
	class EntityShader : public Shader
	{
	public:
		EntityShader()
		{
			loadVertexShader("Resources/Shaders/Entity_VS.glsl");
			loadFragmentShader("Resources/Shaders/Entity_FS.glsl");
			compile();

			mProjViewLoc = registerUniform("uProjView");
			mModelLoc = registerUniform("uModel");
		}
		~EntityShader()
		{
			cleanup();
		}

		void uploadProjView(const Mat4x4& mat) const
		{
			uploadMat4x4(mProjViewLoc, mat);
		}
		void uploadModel(const Mat4x4& mat) const
		{
			uploadMat4x4(mModelLoc, mat);
		}
	private:
		Int32 mProjViewLoc, mModelLoc;
	};

	class EntityRenderer
	{
	public:
		~EntityRenderer();
		void render(GBuffer& gBuffer, const Camera& camera, bool renderCullingSphere);
	private:
		EntityShader mShader;
	};
}