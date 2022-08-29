#pragma once

#include "Core/Data/Camera.hpp"
#include "GBuffer.hpp"
#include "Core/ECS/ECSConstants.hpp"

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
			mSkinnedLoc = registerUniform("uSkinned");

			for (int i = 0; i < ECS::MAX_BONE_MATRICES; i++)
			{
				mBoneMatrices[i] = registerUniform("uBones[" + std::to_string(i) + "]");
			}
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

		void uploadIsSkinned(const bool skinned) const
		{
			uploadInt(mSkinnedLoc, skinned);
		}

		void uploadBoneMatrices(const glm::mat4x4* matrices) const
		{
			for (int i = 0; i < ECS::MAX_BONE_MATRICES; i++)
			{
				uploadMat4x4(mBoneMatrices[i], matrices[i]);
			}
		}
	private:
		Int32 mProjViewLoc, mModelLoc, mSkinnedLoc;
		Int32 mBoneMatrices[ECS::MAX_BONE_MATRICES];
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