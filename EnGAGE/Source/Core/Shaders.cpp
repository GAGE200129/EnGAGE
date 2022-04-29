#include "pch.hpp"
#include "Shaders.hpp"

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

Core::DirectionalShader::DirectionalShader()
{
	this->loadVertexShader("Resources/Shaders/Quad.glsl");
	this->loadFragmentShader("Resources/Shaders/Directional.glsl");
	this->compile();
	this->bind();
	this->uploadInt(this->registerUniform("gPositionTex"), 0);
	this->uploadInt(this->registerUniform("gNormalTex"), 1);
	this->uploadInt(this->registerUniform("gColorTex"), 2);
	mDirectionLoc = this->registerUniform("uDirection");
	mColorLoc = this->registerUniform("uColor");
	mIntensityLoc = this->registerUniform("uIntensity");
	mCamPosLoc = this->registerUniform("uCamPos");
	this->unBind();
}

void Core::DirectionalShader::uploadParams(const glm::vec3& dir, const glm::vec3& color, float intensity, const glm::vec3& camPos)
{
	this->uploadVec3(mDirectionLoc, dir);
	this->uploadVec3(mColorLoc, color);
	this->uploadFloat(mIntensityLoc, intensity);
	this->uploadVec3(mCamPosLoc, camPos);
}

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
	mConstant= this->registerUniform("uConstant");
	mLinear= this->registerUniform("uLinear");
	mExponent= this->registerUniform("uExponent");
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
