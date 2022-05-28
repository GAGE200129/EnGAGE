#pragma once

#include "Core/Shader.hpp"
#include "Core/Camera.hpp"
#include "GBuffer.hpp"

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
	};

	class MapRenderer
	{
	public:
		~MapRenderer();
		void render(GBuffer& gBuffer, const Camera& camera);
	private:
		MapShader mShader;
	};
}