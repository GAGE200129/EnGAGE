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
	private:
		int mViewLoc, mProjLoc;
	};
}