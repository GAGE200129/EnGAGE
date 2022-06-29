#include "pch.hpp"
#include "DirectionalRenderer.hpp"

#include "Core/ECS/ECS.hpp"
#include "Core/ECS/DirectionalLight.hpp"
#include "Core/ECS/Transform.hpp"
#include "Core/ECS/ModelRenderer.hpp"
#include "Core/Math.hpp"
#include "Core/Data/Model.hpp"

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
	this->uploadInt(this->registerUniform("gDepthMap"), 3);
	this->uploadInt(this->registerUniform("uDepthMap"), 4);
	mDirectionLoc = this->registerUniform("uDirection");
	mColorLoc = this->registerUniform("uColor");
	mIntensityLoc = this->registerUniform("uIntensity");
	mCamPosLoc = this->registerUniform("uCamPos");
	mLightProjView = this->registerUniform("uLightProjView");
	mShadowDistance = this->registerUniform("uShadowDistance");
	mShadowFadeStart = this->registerUniform("uShadowFadeStart");
	this->unBind();

}

void Core::DirectionalShader::uploadShadowProjView(const Mat4x4& projView)
{
	uploadMat4x4(mLightProjView, projView);
}

void Core::DirectionalShader::uploadShadowShadowDistanceAndFadeStart(F32 distance, F32 fadeStart)
{
	uploadFloat(mShadowDistance, distance);
	uploadFloat(mShadowFadeStart, fadeStart);
}

void Core::DirectionalShader::uploadParams(const glm::vec3& dir, const glm::vec3& color, float intensity, const glm::vec3& camPos)
{
	this->uploadVec3(mDirectionLoc, dir);
	this->uploadVec3(mColorLoc, color);
	this->uploadFloat(mIntensityLoc, intensity);
	this->uploadVec3(mCamPosLoc, camPos);

}

Core::ShadowMapShader::ShadowMapShader()
{
	this->loadVertexShader("Resources/Shaders/ShadowMap_VS.glsl");
	this->loadFragmentShader("Resources/Shaders/ShadowMap_FS.glsl");
	this->compile();
	this->bind();
	mProjViewLoc = this->registerUniform("uProjView");
	mModelMat = this->registerUniform("uModel");
}

void Core::ShadowMapShader::uploadProjView(const Mat4x4& projView)
{
	uploadMat4x4(mProjViewLoc, projView);
}

void Core::ShadowMapShader::uploadModel(const Mat4x4& modelMat)
{
	uploadMat4x4(mModelMat, modelMat);
}


Core::DirectionalRenderer::DirectionalRenderer(UInt32 shadowSize, F32 renderScale) :
	mShader(), mShadowShader()
{
	glGenFramebuffers(1, &mFBO);
	glGenTextures(1, &mDepthMap);
	resize(shadowSize, renderScale);
}

Core::DirectionalRenderer::~DirectionalRenderer()
{
	glDeleteFramebuffers(1, &mFBO);
	glDeleteTextures(1, &mDepthMap);
	mShader.cleanup();
	mShadowShader.cleanup();
}

void Core::DirectionalRenderer::render(UInt32 width, UInt32 height, GBuffer& gBuffer, const Camera& camera, MapRenderer& mapRenderer, F32 shadowDistance, F32 shadowFadeStart)
{
	System& directionalSystem = ECS::getSystem(SystemType::DIRECTIONAL);

	for (auto e : directionalSystem.entities)
	{
		DirectionalLight::Component* pLight = (DirectionalLight::Component*)ECS::getComponent(e, ComponentType::DIRECTIONAL_LIGHT);
		glViewport(0, 0, mShadowSize, mShadowSize);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		auto lightProjView = Math::calCSMShadowMapProjView(pLight->direction, 10.0f, camera, camera.near, shadowDistance);
		mShadowShader.bind();
		mShadowShader.uploadProjView(lightProjView);
		mShadowShader.uploadModel(Mat4x4(1.0f));
		//Render map
		mapRenderer.renderNoShader();

		//Render entity		
		std::function<void(const Model* model, const Node& node, glm::mat4x4 accumulatedTransform)> processNode;
		processNode = [&](const Model* model, const Node& node, glm::mat4x4 accumulatedTransform)
		{
			accumulatedTransform = glm::translate(accumulatedTransform, node.position);
			accumulatedTransform *= glm::toMat4(node.rotation);
			accumulatedTransform = glm::scale(accumulatedTransform, node.scale);
		
			if (node.meshIndex != -1)
			{
				const Core::Mesh& mesh = model->meshes[node.meshIndex];
				for (const auto& primitive : mesh.primitives)
				{
					Vec3 position;
					position.x = accumulatedTransform[3][0];
					position.y = accumulatedTransform[3][1];
					position.z = accumulatedTransform[3][2];
					mShadowShader.uploadModel(accumulatedTransform);
					glBindVertexArray(primitive.vao);
					glDrawElements(GL_TRIANGLES, primitive.vertexCoumt, primitive.eboDataType, nullptr);
				}
			}
			for (const auto& child : node.children)
			{
				processNode(model, model->nodes[child], accumulatedTransform);
			}
		};

		System& system = ECS::getSystem(SystemType::RENDERER);
		for (auto e : system.entities)
		{
			Transform::Component* pTransform = (Transform::Component*)ECS::getComponent(e, ComponentType::TRANSFORM);
			ModelRenderer::Component* pModelComp = (ModelRenderer::Component*)ECS::getComponent(e, ComponentType::MODEL_RENDERER);
			if (pModelComp->pModel)
			{
				const Model* pModel = pModelComp->pModel;
				Mat4x4 modelMat;
				modelMat = glm::translate(glm::mat4(1.0f), { pTransform->x, pTransform->y, pTransform->z });
				modelMat *= glm::toMat4(glm::quat{ pTransform->rw, pTransform->rx, pTransform->ry, pTransform->rz });
				modelMat = glm::scale(modelMat, { pTransform->sx, pTransform->sy, pTransform->sz });

				processNode(pModel, pModel->nodes[pModel->rootNodeIndex], modelMat);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);


		gBuffer.bindQuad();
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, mDepthMap);
		mShader.bind();	
		mShader.uploadParams(pLight->direction, pLight->color, pLight->intensity, { camera.x, camera.y, camera.z });
		mShader.uploadShadowProjView(lightProjView);
		mShader.uploadShadowShadowDistanceAndFadeStart(shadowDistance, shadowFadeStart);
		gBuffer.renderQuad();
	}

	
}

void Core::DirectionalRenderer::resize(UInt32 shadowSize, F32 renderScale)
{
	mShadowSize = shadowSize * renderScale;
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
		mShadowSize, mShadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


