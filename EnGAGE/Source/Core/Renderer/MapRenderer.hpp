#pragma once

#include "Core/Shader.hpp"
#include "Core/Data/Camera.hpp"
#include "GBuffer.hpp"
#include "Core/Map/WallMesh.hpp"

namespace Core
{
	class MapShader : public Shader
	{
	public:
		MapShader()
		{
			loadVertexShader("Resources/Shaders/Map_VS.glsl");
			loadFragmentShader("Resources/Shaders/Map_FS.glsl");
			compile();

			mProjViewLoc = registerUniform("uProjView");
			mTextureSheet = registerUniform("uTextureSheets");
			bind();
			uploadInt(mTextureSheet, 0);
		}
		~MapShader()
		{
			cleanup();
		}

		void uploadProjView(const Mat4x4& mat) const
		{
			uploadMat4x4(mProjViewLoc, mat);
		}
	private:
		Int32 mProjViewLoc;
		Int32 mTextureSheet;
	};

	class MapRenderer
	{
	public:
		~MapRenderer();
		void render(const Camera& camera);
		void renderNoShader();
	private:
		MapShader mShader;
	};
}