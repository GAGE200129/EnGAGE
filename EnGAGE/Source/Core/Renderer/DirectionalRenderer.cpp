#include "pch.hpp"
#include "DirectionalRenderer.hpp"

#include "Core/ECS.hpp"
#include "Core/Components/DirectionalLight.hpp"

#include <glad/glad.h>

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

Core::DirectionalRenderer::DirectionalRenderer() :
	mShader()
{
}

Core::DirectionalRenderer::~DirectionalRenderer()
{
	mShader.cleanup();
}

void Core::DirectionalRenderer::render(const GBuffer& gBuffer, const Camera& camera)
{
	mShader.bind();
	System& directionalSystem = ECS::getSystem(SystemType::DIRECTIONAL);
	for (auto e : directionalSystem.entities)
	{
		DirectionalLight::Component* pLight = (DirectionalLight::Component*)ECS::getComponent(e, ComponentType::DIRECTIONAL_LIGHT);	
		mShader.uploadParams(pLight->direction, pLight->color, pLight->intensity, { camera.x, camera.y, camera.z });
		gBuffer.renderQuad();
	}
	
}
