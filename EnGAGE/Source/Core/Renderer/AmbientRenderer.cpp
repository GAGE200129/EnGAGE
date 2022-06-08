#include "pch.hpp"
#include "AmbientRenderer.hpp"

#include <glad/glad.h>

Core::AmbientShader::AmbientShader()
{
	this->loadVertexShader("Resources/Shaders/Quad.glsl");
	this->loadFragmentShader("Resources/Shaders/Ambient.glsl");
	this->compile();

	this->bind();
	this->uploadInt(this->registerUniform("gPositionTex"), 0);
	this->uploadInt(this->registerUniform("gNormalTex"), 1);
	this->uploadInt(this->registerUniform("gColorTex"), 2);
	mAmbientLoc = this->registerUniform("uAmbient");
	this->unBind();
}

void Core::AmbientShader::uploadAmbient(const glm::vec3& v)
{
	uploadVec3(mAmbientLoc, v);
}

Core::AmbientRenderer::AmbientRenderer() :
	mAmbientColor(0.3f, 0.3f, 0.3f),
	mShader()
{
}

Core::AmbientRenderer::~AmbientRenderer()
{
	mShader.cleanup();
}


void Core::AmbientRenderer::render(GBuffer& buffer)
{
	buffer.bindQuad();
	mShader.bind();
	mShader.uploadAmbient(mAmbientColor);
	buffer.renderQuad();
}
