#include "pch.hpp"
#include "Renderer.hpp"

#include "ECS.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Model.hpp"
#include "Shaders.hpp"
#include "DebugRenderer.hpp"
#include "Math.hpp"
#include "Components/Transform.hpp"
#include "Components/ModelRenderer.hpp"
#include "Components/DirectionalLight.hpp"
#include "Components/PointLight.hpp"
#include <glad/glad.h>


namespace Core::Renderer
{
	void processNode(const Math::Frustum& frustum, const Model* model, const Node& node, glm::mat4x4 accumulatedTransform);

	void initQuad();
	void initShaders();
	void initGBuffer(UInt32 inWidth, UInt32 inHeight, F32 scale);
	void updateGBuffer(UInt32 inWidth, UInt32 inHeight, F32 scale);

	static Camera gCamera;
	static Vec3 gAmbient = { 0.1f, 0.1f, 0.1f };
	static UInt32 gQuadVAO;
	static UInt32 gQuadVBO;

	static Scope<Shader> gBufferShader;
	static Int32 gProjViewLoc, gModelLoc;

	static Scope<AmbientShader> gAmbientShader;
	static Scope<DirectionalShader> gDirectionalShader;
	static Scope<PointShader> gPointShader;
	
	static UInt32 gFBO, gRBO;
	static UInt32 gPoisitonTex, gNormalTex, gColorTex;

	static bool gRenderCullingSphere = false;
	static F32 gRenderScale = 1.0f;

	void init(UInt32 currentWidth, UInt32 currentHeight)
	{	
		initShaders();
		initGBuffer(currentWidth, currentHeight, gRenderScale);
		initQuad();	
	}
	void onMessage(const Message* pMessage)
	{
		if (auto setScale = Messenger::messageCast<MessageType::RENDERER_SET_SCALE, RendererSetScaleMessage>(pMessage))
		{
			gRenderScale = setScale->scale;
			if (gRenderScale < 0.0f) gRenderScale = 0.0f;
			else if (gRenderScale > 2.0f) gRenderScale = 2.0f;
			updateGBuffer(Window::getWidth(), Window::getHeight(), gRenderScale);
		}
		else if (auto windowResized = Messenger::messageCast<MessageType::WINDOW_RESIZED, WindowResizedMessage>(pMessage))
		{
			updateGBuffer(windowResized->width, windowResized->height, gRenderScale);
		}
		else if (pMessage->type == MessageType::RENDERER_TOGGLE_CULLING_SPHERE)
		{
			gRenderCullingSphere = !gRenderCullingSphere;
		}
	}

	void shutdown()
	{
		gBufferShader->cleanup();
		gAmbientShader->cleanup();
		gDirectionalShader->cleanup();
		gPointShader->cleanup();
		glDeleteFramebuffers(1, &gFBO);
		glDeleteRenderbuffers(1, &gRBO);
		glDeleteTextures(1, &gPoisitonTex);
		glDeleteTextures(1, &gNormalTex);
		glDeleteTextures(1, &gColorTex);
		glDeleteVertexArrays(1, &gQuadVAO);
		glDeleteBuffers(1, &gQuadVBO);
	}

	void render()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, Window::getWidth() * gRenderScale, Window::getHeight() * gRenderScale);
		
		Mat4x4 gProjView = Math::calculateProjectionView();
		gBufferShader->bind();
		gBufferShader->uploadMat4x4(gProjViewLoc, gProjView);
		Math::Frustum frustum = Math::createFrustum();

		//Render all to Geometry buffer
		System& system = ECS::getSystem(SystemType::RENDERER);
		for (auto e : system.entities)
		{
			Transform::Component* pTransform = (Transform::Component*)ECS::getComponent(e, ComponentType::TRANSFORM);
			ModelRenderer::Component* pModelComp = (ModelRenderer::Component*)ECS::getComponent(e, ComponentType::MODEL_RENDERER);
			if (pModelComp->pModel)
			{
				const Model* pModel = pModelComp->pModel;
				glm::mat4x4 modelMat;
				modelMat = glm::translate(glm::mat4(1.0f), { pTransform->x, pTransform->y, pTransform->z });
				modelMat *= glm::toMat4(glm::quat{ pTransform->rw, pTransform->rx, pTransform->ry, pTransform->rz });
				modelMat = glm::scale(modelMat, { pTransform->sx, pTransform->sy, pTransform->sz });

				processNode(frustum, pModel, pModel->nodes[pModel->rootNodeIndex], modelMat);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, Window::getWidth(), Window::getHeight());
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPoisitonTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColorTex);


		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(gQuadVAO);

		//Ambient pass
		gAmbientShader->bind();
		gAmbientShader->uploadAmbient(gAmbient);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Directional pass
		System& directionalSystem = ECS::getSystem(SystemType::DIRECTIONAL);
		for (auto e : directionalSystem.entities)
		{
			DirectionalLight::Component* pLight = (DirectionalLight::Component*)ECS::getComponent(e, ComponentType::DIRECTIONAL_LIGHT);
			gDirectionalShader->bind();
			gDirectionalShader->uploadParams(pLight->direction, pLight->color, pLight->intensity, { gCamera.x, gCamera.y, gCamera.z });
			
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//Point pass
		System& pointSystem = ECS::getSystem(SystemType::POINT);
		for (auto e : pointSystem.entities)
		{
			PointLight::Component* pLight = (PointLight::Component*)ECS::getComponent(e, ComponentType::POINT_LIGHT);
			Transform::Component* pTransform = (Transform::Component*)ECS::getComponent(e, ComponentType::TRANSFORM);
		
			gPointShader->bind();
			gPointShader->uploadParams(pLight->color, 
				{ pTransform->x, pTransform->y, pTransform->z },
				pLight->intensity, 
				pLight->constant,
				pLight->linear,
				pLight->exponent, 
				{ gCamera.x, gCamera.y, gCamera.z });
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
	}
	Camera& getCamera()
	{
		return gCamera;
	}


	static void updateGBuffer(UInt32 inWidth, UInt32 inHeight, F32 scale)
	{
		UInt32 width = inWidth * scale;
		UInt32 height = inHeight * scale;
		glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
		glBindTexture(GL_TEXTURE_2D, gPoisitonTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPoisitonTex, 0);

		glBindTexture(GL_TEXTURE_2D, gNormalTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormalTex, 0);

		glBindTexture(GL_TEXTURE_2D, gColorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorTex, 0);

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		glBindRenderbuffer(GL_RENDERBUFFER, gRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	static void initQuad()
	{
		constexpr float data[] =
		{
			 -1.0f, -1.0f, 0.0f, 0.0f, 
			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, 1.0f, 1.0f,
		};

		glGenVertexArrays(1, &gQuadVAO);
		glGenBuffers(1, &gQuadVBO);

		glBindVertexArray(gQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, gQuadVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	static void initGBuffer(UInt32 inWidth, UInt32 inHeight, F32 scale)
	{
		glGenFramebuffers(1, &gFBO);
		glGenRenderbuffers(1, &gRBO);
		glGenTextures(1, &gPoisitonTex);
		glGenTextures(1, &gNormalTex);
		glGenTextures(1, &gColorTex);
		updateGBuffer(inWidth, inHeight, scale);
	}

	static void initShaders()
	{
		gBufferShader = createScope<Core::Shader>();
		gBufferShader->loadVertexShader("Resources/Shaders/GBuffer_VS.glsl");
		gBufferShader->loadFragmentShader("Resources/Shaders/GBuffer_FS.glsl");
		gBufferShader->compile();

		gProjViewLoc = gBufferShader->registerUniform("uProjView");
		gModelLoc = gBufferShader->registerUniform("uModel");

		gAmbientShader = createScope<Core::AmbientShader>();
		gDirectionalShader = createScope<Core::DirectionalShader>();
		gPointShader = createScope<Core::PointShader>();
	}


	static void processNode(const Math::Frustum& frustum, const Model* model, const Node& node, glm::mat4x4 accumulatedTransform)
	{
		accumulatedTransform = glm::translate(accumulatedTransform, node.position);
		accumulatedTransform *= glm::toMat4(node.rotation);
		accumulatedTransform = glm::scale(accumulatedTransform, node.scale);


		if (node.meshIndex != -1)
		{
			const Core::Mesh& mesh = model->meshes[node.meshIndex];

			
			auto isOnFrustum = [](const Math::Frustum& frustum, const glm::vec3& point, const float radius) -> bool
			{
				auto isOnOrForwardPlan = [&point, &radius](const Vec4& plan) -> bool
				{
					const float distanceToPoint = glm::dot(Vec3(plan), point) + plan.w + radius;
					return (distanceToPoint > 0);
				};
				return (isOnOrForwardPlan(frustum.leftFace) &&
					isOnOrForwardPlan(frustum.rightFace) &&
					isOnOrForwardPlan(frustum.farFace) &&
					isOnOrForwardPlan(frustum.nearFace) &&
					isOnOrForwardPlan(frustum.topFace) &&
					isOnOrForwardPlan(frustum.bottomFace));
			};

			Vec3 position;
			position.x = accumulatedTransform[3][0];
			position.y = accumulatedTransform[3][1];
			position.z = accumulatedTransform[3][2];

			Vec3 scale;
			scale.x = glm::length(Vec3(accumulatedTransform[0]));
			scale.y = glm::length(Vec3(accumulatedTransform[1]));
			scale.z = glm::length(Vec3(accumulatedTransform[2]));
			float scaleFactor = glm::max(scale.z, glm::max(scale.x, scale.y));

			if (gRenderCullingSphere)
			{
				DebugRenderer::addSphere({ 1, 1, 0 }, mesh.boundingSphereRadius * scaleFactor, position);
			}

			//Frustum culling
			if (isOnFrustum(frustum, position, mesh.boundingSphereRadius * scaleFactor))
			{
				for (const auto& primitive : mesh.primitives)
				{
					gBufferShader->uploadMat4x4(gModelLoc, accumulatedTransform);
					glBindVertexArray(primitive.vao);
					glDrawElements(GL_TRIANGLES, primitive.vertexCoumt, primitive.eboDataType, nullptr);
				}
			}
		}
		for (const auto& child : node.children)
		{
			processNode(frustum, model, model->nodes[child], accumulatedTransform);
		}
	}
}






