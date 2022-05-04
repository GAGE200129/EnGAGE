#include "pch.hpp"
#include "Renderer.hpp"

#include "ECS.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Model.hpp"
#include "Shaders.hpp"
#include <glad/glad.h>


namespace Core::Renderer
{
	struct Plane
	{
		glm::vec3 normal;
		float distance;
	};
	struct Frustum
	{
		Plane top, bottom, right, left, far, near;
	};

	void buildProjViewMatrix(const Camera& camera, unsigned int width, unsigned int height, glm::mat4x4& outProjMat, glm::mat4x4& outViewMat);
	void processNode(const Model* model, const Node& node, glm::mat4x4 accumulatedTransform);

	void initQuad();
	void initShaders();
	void initGBuffer(unsigned int currentWidth, unsigned int currentHeight);
	void updateGBuffer(unsigned int width, unsigned int height);

	static Camera gCamera;
	static int gCurrentWidth, gCurrentHeight;
	static glm::vec3 gAmbient;
	static unsigned int gQuadVAO;
	static unsigned int gQuadVBO;

	static Scope<Shader> gBufferShader;
	static int projLoc, viewLoc, modelLoc;

	static Scope<AmbientShader> gAmbientShader;
	static Scope<DirectionalShader> gDirectionalShader;
	static Scope<PointShader> gPointShader;
	
	static unsigned int gFBO, gRBO;
	static unsigned int gPoisitonTex, gNormalTex, gColorTex;

	void init(unsigned int currentWidth, unsigned int currentHeight)
	{
		gAmbient = { 0.1f, 0.1f, 0.1f };
		gCurrentWidth = currentWidth;
		gCurrentHeight = currentHeight;
		
		initShaders();
		initGBuffer(currentWidth, currentHeight);
		initQuad();	
	}
	void onMessage(const Message* pMessage)
	{
		if (pMessage->type == MessageType::WINDOW_RESIZED)
		{
			const int* data = reinterpret_cast<const int*>(pMessage->message);
			int width = data[0];
			int height = data[1];

			gCurrentWidth = width;
			gCurrentHeight = height;
			updateGBuffer(width, height);
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
		static glm::mat4x4 gProj;
		static glm::mat4x4 gView;

		glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, gCurrentWidth, gCurrentHeight);
		buildProjViewMatrix(gCamera, gCurrentWidth, gCurrentHeight, gProj, gView);
		gBufferShader->bind();
		gBufferShader->uploadMat4x4(projLoc, gProj);
		gBufferShader->uploadMat4x4(viewLoc, gView);

		System& system = ECS::getSystem(SystemType::RENDERER);
		for (unsigned int e : system.entities)
		{
			TransformComponent* pTransform = (TransformComponent*)ECS::getComponent(e, ComponentType::TRANSFORM);
			ModelRendererComponent* pModelComp = (ModelRendererComponent*)ECS::getComponent(e, ComponentType::MODEL_RENDERER);
			if (pModelComp->pModel)
			{
				const Model* pModel = pModelComp->pModel;
				glm::mat4x4 modelMat;
				modelMat = glm::translate(glm::mat4(1.0f), { pTransform->x, pTransform->y, pTransform->z });
				modelMat *= glm::toMat4(glm::quat{ pTransform->rw, pTransform->rx, pTransform->ry, pTransform->rz });
				modelMat = glm::scale(modelMat, { pTransform->sx, pTransform->sy, pTransform->sz });

				processNode(pModel, pModel->nodes[pModel->rootNodeIndex], modelMat);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, gCurrentWidth, gCurrentHeight);
		
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

		//Directional
		System& directionalSystem = ECS::getSystem(SystemType::DIRECTIONAL);
		for (unsigned int e : directionalSystem.entities)
		{
			DirectionalLightComponent* pLight = (DirectionalLightComponent*)ECS::getComponent(e, ComponentType::DIRECTIONAL_LIGHT);
			gDirectionalShader->bind();
			gDirectionalShader->uploadParams(pLight->direction, pLight->color, pLight->intensity, { gCamera.x, gCamera.y, gCamera.z });
			
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//Point
		System& pointSystem = ECS::getSystem(SystemType::POINT);
		for (unsigned int e : pointSystem.entities)
		{
			PointLightComponent* pLight = (PointLightComponent*)ECS::getComponent(e, ComponentType::POINT_LIGHT);
			TransformComponent* pTransform = (TransformComponent*)ECS::getComponent(e, ComponentType::TRANSFORM);
		
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


	static void updateGBuffer(unsigned int width, unsigned int height)
	{
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

	static void initGBuffer(unsigned int currentWidth, unsigned int currentHeight)
	{
		glGenFramebuffers(1, &gFBO);
		glGenRenderbuffers(1, &gRBO);
		glGenTextures(1, &gPoisitonTex);
		glGenTextures(1, &gNormalTex);
		glGenTextures(1, &gColorTex);
		updateGBuffer(currentWidth, currentHeight);
	}

	static void initShaders()
	{
		gBufferShader = createScope<Core::Shader>();
		gBufferShader->loadVertexShader("Resources/Shaders/GBuffer_VS.glsl");
		gBufferShader->loadFragmentShader("Resources/Shaders/GBuffer_FS.glsl");
		gBufferShader->compile();

		projLoc = gBufferShader->registerUniform("uProj");
		viewLoc = gBufferShader->registerUniform("uView");
		modelLoc = gBufferShader->registerUniform("uModel");

		gAmbientShader = createScope<Core::AmbientShader>();
		gDirectionalShader = createScope<Core::DirectionalShader>();
		gPointShader = createScope<Core::PointShader>();
	}

	static void buildProjViewMatrix(const Camera& camera, unsigned int width, unsigned int height, glm::mat4x4& outProjMat, glm::mat4x4& outViewMat)
	{
		outViewMat = glm::rotate(glm::mat4(1.0f), -glm::radians(camera.pitch), { 1, 0, 0 });
		outViewMat = glm::rotate(outViewMat, -glm::radians(camera.yaw), { 0, 1, 0 });
		outViewMat = glm::translate(outViewMat, { -camera.x, -camera.y, -camera.z });

		if (height == 0.0f)
		{
			return;
		}
		float aspectRatio = (float)width / (float)height;

		outProjMat = glm::perspective(glm::radians(camera.fov), aspectRatio, camera.near, camera.far);
	}

	static void processNode(const Model* model, const Node& node, glm::mat4x4 accumulatedTransform)
	{
		accumulatedTransform = glm::translate(accumulatedTransform, node.position);
		accumulatedTransform *= glm::toMat4(node.rotation);
		accumulatedTransform = glm::scale(accumulatedTransform, node.scale);


		if (node.meshIndex != -1)
		{
			const Core::Mesh& mesh = model->meshes[node.meshIndex];

			for (const auto& primitive : mesh.primitives)
			{
				gBufferShader->uploadMat4x4(modelLoc, accumulatedTransform);
				glBindVertexArray(primitive.vao);
				glDrawElements(GL_TRIANGLES, primitive.vertexCoumt, primitive.eboDataType, nullptr);
			}
		}
		for (const auto& child : node.children)
		{
			processNode(model, model->nodes[child], accumulatedTransform);
		}
	}
}






