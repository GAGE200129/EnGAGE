#include "pch.hpp"
#include "PointRenderer.hpp"

#include <glad/glad.h>

Core::PointShader::PointShader()
{
	this->loadVertexShader("Resources/Shaders/Quad.glsl");
	this->loadFragmentShader("Resources/Shaders/Point.glsl");
	this->compile();
	this->bind();
	this->uploadInt(this->registerUniform("gPositionTex"), 0);
	this->uploadInt(this->registerUniform("gNormalTex"), 1);
	this->uploadInt(this->registerUniform("gColorTex"), 2);
	mColor = this->registerUniform("uColor");
	mPosition = this->registerUniform("uPosition");
	mIntensity = this->registerUniform("uIntensity");
	mConstant = this->registerUniform("uConstant");
	mLinear = this->registerUniform("uLinear");
	mExponent = this->registerUniform("uExponent");
	mCamPos = this->registerUniform("uCamPos");
	this->unBind();
}

void Core::PointShader::uploadParams(const glm::vec3& color, const glm::vec3& position, float intensity, float constant, float linear, float exponent, const glm::vec3& camPos)
{
	uploadVec3(mColor, color);
	uploadVec3(mPosition, position);
	uploadVec3(mCamPos, camPos);
	uploadFloat(mIntensity, intensity);
	uploadFloat(mConstant, constant);
	uploadFloat(mExponent, exponent);
}


Core::PointRenderer::PointRenderer() :
	mShader()
{
}

Core::PointRenderer::~PointRenderer()
{
	mShader.cleanup();
}

void Core::PointRenderer::render(const glm::vec3& color, const glm::vec3& position, float intensity, float constant, float linear, float exponent, const glm::vec3& camPos)
{
	mShader.bind();
	mShader.uploadParams(color, position, intensity, constant, linear, exponent, camPos);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
