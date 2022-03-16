#include "pch.hpp"
#include "TestShader.hpp"

Core::TestShader::TestShader()
{
	this->loadVertexShader("Resources/Shaders/test_VS.glsl");
	this->loadFragmentShader("Resources/Shaders/test_FS.glsl");
	this->compile();

	mViewLoc = registerUniform("uView");
	mProjLoc = registerUniform("uProj");
	mModelLoc = registerUniform("uModel");
}

void Core::TestShader::uploadViewMat(const glm::mat4x4& view)
{
	uploadMat4x4(mViewLoc, view);
}

void Core::TestShader::uploadProjMat(const glm::mat4x4& proj)
{
	uploadMat4x4(mProjLoc, proj);
}

void Core::TestShader::uploadModelMat(const glm::mat4x4& model)
{
	uploadMat4x4(mModelLoc, model);
}
