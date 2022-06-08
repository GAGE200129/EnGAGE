#include "pch.hpp"
#include "PointRenderer.hpp"


#include "Core/ECS.hpp"
#include "Core/Components/PointLight.hpp"
#include "Core/Components/Transform.hpp"


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

void Core::PointRenderer::render(GBuffer& gBuffer, const Camera& camera)
{
	gBuffer.bindQuad();
	mShader.bind();
	System& pointSystem = ECS::getSystem(SystemType::POINT);
	for (auto e : pointSystem.entities)
	{
		PointLight::Component* pLight = (PointLight::Component*)ECS::getComponent(e, ComponentType::POINT_LIGHT);
		Transform::Component* pTransform = (Transform::Component*)ECS::getComponent(e, ComponentType::TRANSFORM);

		
		mShader.uploadParams(pLight->color,
			{ pTransform->x, pTransform->y, pTransform->z },
			pLight->intensity,
			pLight->constant,
			pLight->linear,
			pLight->exponent,
			{ camera.x, camera.y, camera.z });
		gBuffer.renderQuad();
	}

}
