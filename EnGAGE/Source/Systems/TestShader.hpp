#pragma once

#include "Core/Shader.hpp"

namespace Core
{
	class TestShader : public Shader
	{
	public:
		TestShader();

		void uploadViewMat(const glm::mat4x4& view);
		void uploadProjMat(const glm::mat4x4& proj);
		void uploadModelMat(const glm::mat4x4& model);
	private:
		int mViewLoc, mProjLoc, mModelLoc;
	};
}