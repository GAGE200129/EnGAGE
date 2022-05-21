#pragma once

#include "Core/Shader.hpp"

namespace Core
{
	class GBufferShader : public Shader
	{
	public:
		GBufferShader()
		{
			loadVertexShader("Resources/Shaders/GBuffer_VS.glsl");
			loadFragmentShader("Resources/Shaders/GBuffer_FS.glsl");
			compile();

			mProjViewLoc = registerUniform("uProjView");
			mModelLoc = registerUniform("uModel");
		}
		~GBufferShader()
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

	class GBuffer
	{
	public:
		GBuffer();		
		~GBuffer();
		void update(UInt32 inWidth, UInt32 inHeight, F32 scale);

		void bind(UInt32 inWidth, UInt32 inHeight, F32 scale);

		void unBind(UInt32 inWidth, UInt32 inHeight);


		void bindQuad();
		void renderQuad() const;
		void unBindQuad();

		inline void uploadProjView(const Mat4x4& projView)
		{
			shader.uploadProjView(projView);
		}
		inline void uploadModel(const Mat4x4& model)
		{
			shader.uploadModel(model);
		}

		inline void bindShader()
		{
			shader.bind();
		}

		inline const GBufferShader& getShader() const { return shader; };
	private:
		GBufferShader shader;
		UInt32 quadVAO;
		UInt32 quadVBO;
		UInt32 FBO, RBO;
		UInt32 positionTex, normalTex, colorTex;
	};
}